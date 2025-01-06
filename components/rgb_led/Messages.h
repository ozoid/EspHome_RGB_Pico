#ifndef MESSAGES_H
#define MESSAGES_H
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "pico/stdlib.h"
#include "pico/multicore.h"

//-------------------------------------------------------------------------------------------
struct cMessage {
    char mode = '-';
    int opt = 0; // group x or queue x identifier
    int opt1 = 0; // queue position x identifier
    uint8_t pattern = 0;
    char* value;  // just parameter
    uint32_t color = 0;
    uint8_t speed = 0;
    uint8_t repeat = 0;
    uint8_t direction = 0;
    uint8_t win_no = 0;
    bool isvalid = false;
};
//-------------------------------------------------------------------------------------------
typedef struct hsl {
    int h, s, l;
 } HSL;
//-------------------------------------------------------------------------------------------
typedef struct rgb {
    int r, g, b;
 } RGB;
//-------------------------------------------------------------------------------------------
typedef struct combination {
    uint8_t pat,val,rpt,dir;
} COMB;


static inline uint32_t combine(uint8_t pat, uint8_t val, uint8_t rpt, uint8_t dir) {
    return
    ((uint32_t) (dir) <<24) | ((uint32_t) (rpt) <<16) | ((uint32_t) (val) <<8) | (uint32_t) (pat);         
 }
//-------------------------------------------------------------------------------------------
static inline uint32_t combine(COMB c) {
    return
    ((uint32_t) (c.dir) <<24) | ((uint32_t) (c.rpt) <<16) | ((uint32_t) (c.val) <<8) | (uint32_t) (c.pat);         
 }
//-------------------------------------------------------------------------------------------
static COMB uncombine(uint32_t combval) {
    COMB c;
    c.pat = combval & 0xff;
    c.val = (combval >> 8) & 0xff;
    c.rpt = (combval >> 16) & 0xff;
    c.dir = (combval >> 24) & 0xff;
    return c;
 }
//-------------------------------------------------------------------------------------------
static void core1_pushData(uint32_t comb, uint32_t color, uint32_t comb2){
    multicore_fifo_push_blocking(comb);
    multicore_fifo_push_blocking(color);
    multicore_fifo_push_blocking(comb2);
}
//-------------------------------------------------------------------------------------------
static void core1_pushData(uint8_t pattern, uint8_t value,uint8_t win_no, uint32_t color, uint8_t speed, uint8_t repeat, uint8_t direction){
    uint8_t result = 1;
    uint32_t comb = combine(pattern, value, repeat, direction);
    uint32_t comb2 = combine(win_no,speed,0,0);
    core1_pushData(comb,color,comb2);
    result = 0;
}
 
 //-------------------------------------------------------------------------------------------
/// @brief rescale a number to a diiferent range
static inline uint8_t ReScale(int value,int min,int max,int nmin,int nmax){
    //NewValue = (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin
    return (((value - min)*(nmax - nmin))/(max - min)) + nmin;
 } 
 //-------------------------------------------------------------------------------------------
/// @brief GRB 
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
    ((uint32_t) (g) << 16) |
    ((uint32_t) (r) << 8) |
    (uint32_t) (b);
 }
//-------------------------------------------------------------------------------------------
static inline uint32_t urgb_u32(RGB rgb) {
    return
    ((uint32_t) (rgb.g) << 16) |
    ((uint32_t) (rgb.r) << 8) |
    (uint32_t) (rgb.b);
 }
 //-------------------------------------------------------------------------------------------
static RGB u32_RGB(uint32_t u){
    RGB result;
    result.b = u & 0xff;
    result.r = (u >> 8) & 0xff;
    result.g = (u >> 16) & 0xff;
    return result;
 }
//------------------------------------------------------------------------------------------- 
static uint32_t rgbBrightness(uint32_t color,uint8_t bright){
    RGB rgb = u32_RGB(color);
    rgb.r = rgb.r * bright / 100;
    rgb.g = rgb.g * bright / 100;
    rgb.b = rgb.b * bright / 100;
    return urgb_u32(rgb);
 }
//-------------------------------------------------------------------------------------------
static uint32_t hsl2rgb360(float hue, uint8_t saturation, uint8_t value) {
  // Convert hue from 0-360 range to 0-255 range
  uint8_t hueScaled = (uint8_t)(hue / 360.0 * 255.0);
  // Convert HSB (Hue, Saturation, Brightness/Value) to RGB
  uint8_t r, g, b;
  float h = hueScaled / 255.0;
  float s = saturation / 255.0;
  float v = value / 255.0;
  float f = h * 6.0 - floor(h * 6.0);
  float p = v * (1.0 - s);
  float q = v * (1.0 - f * s);
  float t = v * (1.0 - (1.0 - f) * s);
  switch ((int)(h * 6.0)) {
    case 0: r = v * 255.0; g = t * 255.0; b = p * 255.0; break;
    case 1: r = q * 255.0; g = v * 255.0; b = p * 255.0; break;
    case 2: r = p * 255.0; g = v * 255.0; b = t * 255.0; break;
    case 3: r = p * 255.0; g = q * 255.0; b = v * 255.0; break;
    case 4: r = t * 255.0; g = p * 255.0; b = v * 255.0; break;
    case 5: r = v * 255.0; g = p * 255.0; b = q * 255.0; break;
  }
  return urgb_u32(r,g,b);
 }
//------------------------------------------------------------------------------------------- 
// static uint32_t HexToInt(const std::string &data){
//     uint32_t hexNumber;
//     sscanf(data.c_str(), "%x", &hexNumber);
//     return hexNumber;
//  }
//------------------------------------------------------------------------------------------- 
static std::string IntToHex(const uint8_t *data, int len){
    std::stringstream ss;
    ss << std::hex;
    for( int i(0) ; i < len; ++i ){
        ss << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    return ss.str();
}
//-------------------------------------------------------------------------------------------
static std::vector<std::string> SplitStringByDelimeter(const std::string& input, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(input);
	while (getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}
#endif