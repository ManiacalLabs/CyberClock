#include "wifi.h"

#include "globals.h"
#include "char.h"
#include "seg.h"
#include "anims.h"

void setup() {
  Serial.begin(115200);

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);

  // fill_rainbow(rainbow, RAINBOW_COUNT, 0, 256 / RAINBOW_COUNT);
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  FastLED.delay(10);
  
  wifi_setup();
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
}

uint8_t res = 0;
void loop() {
  res = show_time();
  if(res == 2)
  {
    x_wipe_i = 0;
    x_wipe_dir = 1;
    while(!do_x_wipe()){}
  }
  else if(res == 3)
  {
    snake_i = 0;
    snake_dir = 1;
    while(!do_snake()){}
  }

  check_wifi_status();
}