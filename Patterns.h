#ifndef PATTERNS_H
#define PATTERNS_H
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <cmath>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"

class Patterns
{
private:
    uint8_t MAX_COLOR_VALUE = 255;
    int usLEDSleep = 0;
    uint32_t lastcomb;
    uint32_t lastcol;
    uint8_t lastspd;
    uint8_t lastrpt;
    uint8_t lastpat;
    uint8_t lastdir;
    //-------------------------------------------------------------------------------------------
    static inline void put_pixel(uint32_t pixel_grb)
    {
        pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
    }
 public:
    const uint8_t currentSequenceLength = 16;
    uint8_t brightness = 30;
    uint32_t output_array[16]={}; // written to leds - primary animated array buffer
    bool animating = false;
    void core1_entry();
    bool check_incoming_command();
    void cycleRainbow(int delay);
    void clear_pattern(bool doShow);
    void forward_backward(int delay);
    void basic_outline(uint32_t color,int delay);
    void up_down(int delay);
    void basic_cycle(uint32_t color,int delay);
    void round_cycle(uint32_t color,int delay);
    void clock(uint32_t color,int delay);
    void runPattern(uint8_t pattern, uint8_t value, uint32_t color, uint8_t speed, uint8_t repeat, uint8_t dir);
    void show();
    void alloff();
    uint8_t pushValues(uint8_t pattern, uint8_t value, uint32_t color, uint32_t speed, uint8_t repeat, uint8_t direction,bool force);
};




#endif