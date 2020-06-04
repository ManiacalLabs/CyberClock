#ifndef GLOBALS_H
#define GLOBALS_H

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#include "time.h"

#define DATA_PIN    27
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    21 * 4
#define LED_PER_SEG 3

#define OFF CRGB(0,0,0)

CRGB leds[NUM_LEDS];

// #define RAINBOW_COUNT 10
// CRGB rainbow[RAINBOW_COUNT];

#define BRIGHTNESS          64
#define FRAMES_PER_SECOND  120

const char* ntpServer = "time.windows.com";
const long  gmtOffset_sec = -18000;
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

#endif GLOBALS_H