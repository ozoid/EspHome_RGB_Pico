#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <queue>
#include "rgb_led.h"
#include "esphome/core/log.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/stdio.h"
#include "ws2812/generated/ws2812.pio.h"
//#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "Patterns.h"
#include "Messages.h"
//===================================
#define IS_RGBW false
#define DATAOUT1 18
#define CLKOUT1 19
#define DATAOUT2 20
#define CLKOUT2 21
//- Crypto
#define I2C0_SDA_PIN 26
#define I2C0_SCL_PIN 27
//- RTC
#define I2C1_SDA_PIN 14
#define I2C1_SCL_PIN 15
// SPI configuration - FRAM
#define SPI_PORT            spi1
#define PIN_SCK             10
#define PIN_MOSI            11
#define PIN_MISO            12
#define PIN_CS              13

#define SPI_BAUD_RATE       1000000 // 1 MHz
//===================================
namespace esphome{
    namespace rgb_led{
        static Patterns patterns;
        static const char *TAG = "rgb_led";
        void RGBLEDOutput::setup() {
            stdio_init_all();
            initGPIO();
            initPIO();    // for led control
            initI2C();  
        }
        void RGBLEDOutput::loop() {
          //  output_->publish_state(3.0);
        }
      
        void RGBLEDOutput::dump_config() { 
            ESP_LOGCONFIG(TAG, "RGB Number Three"); 
            LOG_NUMBER("  ","Output",output_);
            LOG_NUMBER("  ","Pattern",pattern_);
            LOG_NUMBER("  ","Speed",speed_);
            LOG_NUMBER("  ","Repeat",repeat_);
            LOG_NUMBER("  ","Color",color_);
            LOG_STR(("Brightness" + std::to_string(bright_)).c_str());
            std::string outtxt = "";
            for (int i = 0;i < 16;i++){
                outtxt += std::to_string(i);
                outtxt += " ";
                outtxt += std::to_string(patterns.output_array[i]);
                outtxt += "\n";
            }
            LOG_STR(outtxt.c_str());
        }

        void RGBLEDOutput::set_output(number::Number *output){ output_ = output; pushCurrent(); }
        void RGBLEDOutput::set_pattern(number::Number *pattern){ pattern_ = pattern; pushCurrent(); }
        void RGBLEDOutput::set_color(number::Number *color){ color_ = color; pushCurrent(); }
        void RGBLEDOutput::set_speed(number::Number *speed){ speed_ = speed; pushCurrent(); }
        void RGBLEDOutput::set_repeat(number::Number *repeat){ repeat_ = repeat; pushCurrent(); }
        void RGBLEDOutput::set_brightness(int bright){ bright_ = bright; patterns.brightness = bright; }

        void RGBLEDOutput::pushCurrent(){
            int pstate = pattern_->state;
            int ostate = output_->state;
            int cstate = color_->state;
            int sstate = speed_->state;
            int rstate = repeat_->state;
            patterns.brightness = bright_;
            patterns.pushValues(pstate,ostate,cstate,sstate,rstate,0,false );
        }
        //-------------------------------------------------------------------------------------------
        /// @brief initialise input output pins
        void initGPIO()
        {
            gpio_init(DATAOUT1);
            gpio_set_dir(DATAOUT1, GPIO_OUT);
            gpio_init(DATAOUT2);
            gpio_set_dir(DATAOUT2, GPIO_OUT);
            gpio_init(CLKOUT1);
            gpio_set_dir(CLKOUT1, GPIO_OUT);
            gpio_init(CLKOUT2);
            gpio_set_dir(CLKOUT2, GPIO_OUT);
        }
        //-------------------------------------------------------------------------------------------
        /// @brief initialise I2C for TRUSTM & RTC
        void initI2C()
        {
            uint baud = i2c_init(i2c1, 100 * 1000);
            gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
            gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
            gpio_pull_up(I2C0_SDA_PIN);
            gpio_pull_up(I2C0_SCL_PIN);

            gpio_set_function(I2C1_SDA_PIN, GPIO_FUNC_I2C);
            gpio_set_function(I2C1_SCL_PIN, GPIO_FUNC_I2C);
            gpio_pull_up(I2C1_SDA_PIN);
            gpio_pull_up(I2C1_SCL_PIN);
        }
        //-------------------------------------------------------------------------------------------
        void initSPI()
        {
            // Initialize SPI
            spi_init(SPI_PORT, SPI_BAUD_RATE);
            // spi_set_format(SPI_PORT,8,SPI_CPOL_1,SPI_CPHA_1,SPI_MSB_FIRST);
            gpio_set_function(PIN_MISO, GPIO_FUNC_SPI); // rx
            gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
            gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI); // tx
            // Chip select pin configuration
            gpio_init(PIN_CS);
            gpio_set_dir(PIN_CS, GPIO_OUT);
            gpio_put(PIN_CS, 1); // Set CS high
        }
        //-------------------------------------------------------------------------------------------
        void initPIO()
        {
            PIO pio = pio0;
            uint offset = pio_add_program(pio, &ws2812_program);
            uint sm = pio_claim_unused_sm(pio, true);
            ws2812_program_init(pio, sm, offset, DATAOUT1, 800000, IS_RGBW);

            // uint offset = pio_add_program(pio, &rgb_led_mini_program);
            // rgb_led_mini_program_init(pio, sm, offset, SERIAL_FREQ, PIN_CLK, PIN_DIN);
        }
        //-------------------------------------------------------------------------------------------
        /// @brief main entry point cpu core 1
        void core1_entry()
        {
            uint32_t col = 0;
            uint32_t cmb2 = 0;
            uint32_t comb = 0;
            int packetcount = 0;
            while (true)
            {
                if (multicore_fifo_rvalid() && packetcount == 0)
                {
                    comb = multicore_fifo_pop_blocking();
                    packetcount++;
                }
                if (multicore_fifo_rvalid() && packetcount == 1)
                {
                    col = multicore_fifo_pop_blocking();
                    packetcount++;
                }
                if (multicore_fifo_rvalid() && packetcount == 2)
                {
                    cmb2 = multicore_fifo_pop_blocking();
                    packetcount++;
                }

                if(packetcount == 3){
                    packetcount = 0;
                    if (comb == 0 && col == 0 && cmb2 == 0)
                    {
                        patterns.clear_pattern(false);
                        return;
                    }
                    COMB c = uncombine(comb);
                    COMB c2 = uncombine(cmb2);
                    patterns.runPattern(c.pat, c.val, col, c2.val, c.rpt, c.dir);
                }
            }
        }
        //-------------------------------------------------------------------------------------------
        void initMultiCore() {
            multicore_launch_core1(core1_entry);
            multicore_fifo_push_blocking(0);
            multicore_fifo_push_blocking(0);
            multicore_fifo_push_blocking(0);
        }
    }
}