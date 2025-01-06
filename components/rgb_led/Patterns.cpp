#include "Patterns.h"
#include "Messages.h"
 //-------------------------------------------------------------------------------------------
/// @brief transfer array of colours to physical light string
void Patterns::show()
{
    int k = 0;
    int n = currentSequenceLength;

    for (int i = 0; i < n; i++)
    {
        put_pixel(output_array[i]);
    }
}
//-------------------------------------------------------------------------------------------
bool Patterns::check_incoming_command(){
    if (multicore_fifo_rvalid())
    {
        animating = false;
        return true;
    }
    return false;
}
//-------------------------------------------------------------------------------------------
//-[0]-
void Patterns::clear_pattern(bool doShow)
{
    uint8_t n = currentSequenceLength;
    for (int i = 0; i < n; i++)
    {
        output_array[i] = 0;
    }
    if (doShow)
    {
        show();
    }
}
//-------------------------------------------------------------------------------------------
void Patterns::basic_outline(uint32_t color,int delay) 
{
    while(animating){
        clear_pattern(false);
        sleep_ms(delay);
        uint8_t n = currentSequenceLength;
        for(int i=0;i<n;i+=8){
            output_array[i] = color;
        }
        if (check_incoming_command()){ return; }
        show();
        sleep_ms(delay*10);
    }
}
//-------------------------------------------------------------------------------------------
void Patterns::clock(uint32_t color,int delay){
    int hour = 4;
    int minute = 27;
    int second = 45;
    uint8_t n = currentSequenceLength;
    int hournum = 0;
    int minnum = 0;
    int secnum = 0;
    int mon,rem,sec;

    while(animating){
        clear_pattern(false);
        hournum = ((hour-1) * 8);
        secnum = ((second / 5) * 8);
        minnum = ((minute / 5) * 8);
        rem = minute % 5;
        mon = second % 5;
        sec = second % 12;

        output_array[hournum] = urgb_u32(255,0,0);
        output_array[minnum + rem + 1] = urgb_u32(0,255,0);
        output_array[secnum + mon + 1] = urgb_u32(255,255,0);
        output_array[(sec*8)+7] = urgb_u32(0,0,255);

        second++;
        if(second > 59){
            minute++;
            second = 0;
        }
        if(minute > 59){
            hour++;
            minute = 0;
        }
        if(hour > 11){
            hour = 0;
        }
        show();
        sleep_ms(1000);
    }

}
//-------------------------------------------------------------------------------------------
void Patterns::round_cycle(uint32_t color,int delay){
 uint8_t n = currentSequenceLength;
    while(animating)
    {
        for (int i = 0; i < 12; i++)
        {
            output_array[0+(i*7)] = 0;
            output_array[1+(i*7)] = 0;
            output_array[2+(i*7)] = color;
            output_array[3+(i*7)] = color;
            output_array[4+(i*7)] = color;
            output_array[5+(i*7)] = 0;
            output_array[6+(i*7)] = 0;
        }
            if (check_incoming_command()){ return; }
            show();
            sleep_ms(delay);
        for (int i = 0; i < 12; i++)
        {
            output_array[0+(i*7)] = color;
            output_array[1+(i*7)] = 0;
            output_array[2+(i*7)] = 0;
            output_array[3+(i*7)] = color;
            output_array[4+(i*7)] = 0;
            output_array[6+(i*7)] = color;
            output_array[5+(i*7)] = 0;
        }
            if (check_incoming_command()){ return; }
            show();
            sleep_ms(delay);
        for (int i = 0; i < 12; i++)
        {
            output_array[0+(i*7)] = 0;
            output_array[1+(i*7)] = color;
            output_array[2+(i*7)] = 0;
            output_array[3+(i*7)] = color;
            output_array[4+(i*7)] = 0;
            output_array[6+(i*7)] = 0;
            output_array[5+(i*7)] = color;
        }
            if (check_incoming_command()){ return; }
            show();
            sleep_ms(delay);
    }
}
//-------------------------------------------------------------------------------------------
//-[1]-
void Patterns::basic_cycle(uint32_t color,int delay){
    uint8_t n = currentSequenceLength;
    while(animating){
        for (int i = 0; i < n; i++)
        {
            output_array[i] = color;
            sleep_ms(delay);
            if (check_incoming_command()){ return; }
            show();
        }
         sleep_ms(delay*10);
        for (int i = 0; i < n; i++)
        {
            output_array[i] = 0;
            sleep_ms(delay);
            if (check_incoming_command()){ return; }
            show();
        }
    }
}
//-------------------------------------------------------------------------------------------
//-[2]-
void Patterns::cycleRainbow(int delay)
{
    // Set saturation and value to full brightness
    uint8_t saturation = 255;
    uint8_t value = ReScale(brightness, 0, 100, 0, 255);
    uint8_t n = currentSequenceLength;
    // Cycle through hues from 0 to 360 degrees in steps of 5 degrees
    while(animating){
        for (float hue = 0.0; hue < 360.0; hue += 5) //(360/n)*2)
        {
            for (int i = 0; i < n; i++)
            {
                output_array[i] = hsl2rgb360(hue, saturation, value);
                show();
                sleep_ms(delay);
                if (check_incoming_command()) { return; }
            }
        }
    }
}
//-------------------------------------------------------------------------------------------
//-[3]-
void Patterns::forward_backward(int delay){
uint32_t color =  urgb_u32(255,0,0);
uint8_t n = currentSequenceLength;
    while(animating){
        clear_pattern(false);
        output_array[0] = color;
        output_array[15] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[1] = color;
        output_array[8] = color;
        output_array[14] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[2] = color;
        output_array[7] = color;
        output_array[13] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[3] = color;
        output_array[6] = color;
        output_array[9] = color;
        output_array[12] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[4] = color;
        output_array[5] = color;
        output_array[10] = color;
        output_array[11] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[3] = color;
        output_array[6] = color;
        output_array[9] = color;
        output_array[12] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[2] = color;
        output_array[7] = color;
        output_array[13] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[1] = color;
        output_array[8] = color;
        output_array[14] = color;
        show();
        sleep_ms(delay);
        if (check_incoming_command()) { return; }
    }
}
//-------------------------------------------------------------------------------------------
//-[4]-
void Patterns::up_down(int delay){
uint32_t color =  urgb_u32(255,0,0);
uint8_t n = currentSequenceLength;
    while(animating){
        clear_pattern(false);
        output_array[2] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[1] = color;
        output_array[3] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[0] = color;
        output_array[4] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[5] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[6] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[7] = color;
        output_array[8] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[9] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[10] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[11] = color;
        output_array[15] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[12] = color;
        output_array[14] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[13] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[12] = color;
        output_array[14] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[11] = color;
        output_array[15] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[10] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[9] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
        clear_pattern(false);
        output_array[7] = color;
        output_array[8] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[6] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[5] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[0] = color;
        output_array[4] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
          clear_pattern(false);
        output_array[1] = color;
        output_array[3] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
         clear_pattern(false);
        output_array[2] = color;
        show();
        sleep_ms(delay);
if (check_incoming_command()) { return; }
    }

}
//-------------------------------------------------------------------------------------------
void Patterns::runPattern(uint8_t pattern, uint8_t value, uint32_t color, uint8_t speed, uint8_t repeat, uint8_t dir){
uint8_t psl = currentSequenceLength;
    if (repeat == 0)
    {
        repeat = 1;
    }
    animating = true;
    //printf("Run Pattern %u %u %u %u %u %u pps:%d\n", pattern, value, win_no, speed, repeat, dir, pixels_per_segment);
    if (brightness < 100)
    {
        color = rgbBrightness(color, brightness);
    }
    switch (pattern)
    {
    case 0: // clear
        clear_pattern(true);
        break;
    case 1:
        basic_cycle(color,2);
        break;
    case 2:
        cycleRainbow(5);
        break;
    case 3:
        forward_backward(250);
        break;
    case 4:
        up_down(250);
        break;
     case 5:
        round_cycle(color,185);
        break;
     case 6:
        basic_outline(color,585);
        break;
     case 7:
        clock(color,585);
        break;
    }

}
//-------------------------------------------------------------------------------------------
void Patterns::alloff()
{
    int k = 0;
    int n = currentSequenceLength;
    for (int i = 0; i < n; i++)
    {
            put_pixel(0);
    }
}


//-------------------------------------------------------------------------------------------
/** @brief Push values to cpu core 1
 * @param pattern The pattern number
 * @param value The command/option
 * @param color the RGB color,
 * @param speed how fast to play pattern 0-100
 * @param repeat how many times to repeate pattern
 * @param direction which way to roll (if applicable)
 */
uint8_t Patterns::pushValues(uint8_t pattern, uint8_t value, uint32_t color, uint32_t speed, uint8_t repeat, uint8_t direction,bool force = false)
{
    uint8_t result = 1;
    uint32_t comb = combine(pattern, value, repeat, direction);
    uint32_t comb2 = combine(0, speed, 0, 0);
    // patterns.active_queue = fram_read_queue(value);
    if (comb != lastcomb || color != lastcol || speed != lastspd || repeat != lastrpt || pattern != lastpat || force)
    {
        core1_pushData(comb, color, comb2);
        result = 0;
    }
    lastcol = color;
    lastspd = speed;
    lastcomb = comb;
    lastrpt = repeat;
    lastdir = direction;
    lastpat = pattern;
    return result;
}