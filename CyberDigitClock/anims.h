#include "globals.h"
#include "char.h"

#define Y_RAINBOW_COUNT MATRIX_Y
CRGB y_rainbow[Y_RAINBOW_COUNT];
uint8_t m_y_count = 0;

void fill_y_rainbow(){
  static uint8_t x, y;
  fill_rainbow(y_rainbow, Y_RAINBOW_COUNT, m_y_count, 256 / Y_RAINBOW_COUNT);

  for(y=0; y<MATRIX_Y; y++){
    for(x=0; x<MATRIX_X; x++){
      if(_char_matrix[y][x] >= 0){
        leds[_char_matrix[y][x]] = y_rainbow[y];
      }
    }
  }
}

#define X_RAINBOW_COUNT MATRIX_X
CRGB x_rainbow[X_RAINBOW_COUNT];
uint8_t m_x_count = 0;

int8_t snake_i = 0;
int8_t snake_dir = 1;
bool do_snake()
{
  static uint8_t i;
  fill_y_rainbow();
  m_y_count += 3;

  for(i=0; i<NUM_LEDS; i++){
    if(i == snake_i){ leds[i] = leds[i]; } //basically a no-op
    else if(i == (snake_i - 1*snake_dir)){ leds[i] = leds[i].nscale8_video(192); }
    else if(i == (snake_i - 2*snake_dir)){ leds[i] = leds[i].nscale8_video(128); }
    else if(i == (snake_i - 3*snake_dir)){ leds[i] = leds[i].nscale8_video(96); }
    else if(i == (snake_i - 4*snake_dir)){ leds[i] = leds[i].nscale8_video(64); }
    else { leds[i] = OFF; }
  }

  FastLED.show();
  FastLED.delay(26);

  snake_i+=snake_dir;
  if(snake_i >= NUM_LEDS + 6){snake_dir = -1; snake_i = NUM_LEDS;}
  if(snake_i < -5){snake_dir = 1; snake_i = 0; return true;}
  return false;
}

void fill_row(int8_t row, CRGB color){
  static int8_t x;
  for(x=0; x<MATRIX_X; x++){
    leds[_char_matrix[row][x]] = color;
  }
}

int8_t y_wipe_i = 0;
int8_t y_wipe_dir = 1;
bool do_y_wipe(){
  static uint8_t y;
  fill_rainbow(y_rainbow, Y_RAINBOW_COUNT, m_y_count, 256 / Y_RAINBOW_COUNT);

  for(y=0; y<MATRIX_Y; y++){
    if(y == y_wipe_i){ fill_row(y, y_rainbow[y]); }
    else if(y == (y_wipe_i - 1*snake_dir)){ fill_row(y, y_rainbow[y].nscale8_video(192)); }
    else if(y == (y_wipe_i - 2*snake_dir)){ fill_row(y, y_rainbow[y].nscale8_video(128)); }
    else if(y == (y_wipe_i - 3*snake_dir)){ fill_row(y, y_rainbow[y].nscale8_video(96)); }
    else if(y == (y_wipe_i - 4*snake_dir)){ fill_row(y, y_rainbow[y].nscale8_video(64)); }
    else { fill_row(y, OFF); }
  }

  FastLED.show();
  FastLED.delay(120);

  y_wipe_i+=y_wipe_dir;
  if(y_wipe_i >= MATRIX_Y + 7){y_wipe_dir = -1; y_wipe_i = MATRIX_Y;}
  if(y_wipe_i < -6){y_wipe_dir = 1; y_wipe_i = 0; return true; }
  return false;
}

void fill_col(int8_t col, CRGB color){
  static int8_t y;
  for(y=0; y<MATRIX_Y; y++){
    leds[_char_matrix[y][col]] = color;
  }
}

int8_t x_wipe_i = 0;
int8_t x_wipe_dir = 1;
bool do_x_wipe(){
  static uint8_t x;
  fill_rainbow(x_rainbow, X_RAINBOW_COUNT, m_x_count, 256 / X_RAINBOW_COUNT);

  for(x=0; x<MATRIX_X; x++){
    if(x == x_wipe_i){ fill_col(x, x_rainbow[x]); }
    else if(x == (x_wipe_i - 1*snake_dir)){ fill_col(x, x_rainbow[x].nscale8_video(192)); }
    else if(x == (x_wipe_i - 2*snake_dir)){ fill_col(x, x_rainbow[x].nscale8_video(128)); }
    else if(x == (x_wipe_i - 3*snake_dir)){ fill_col(x, x_rainbow[x].nscale8_video(96)); }
    else if(x == (x_wipe_i - 4*snake_dir)){ fill_col(x, x_rainbow[x].nscale8_video(64)); }
    else { fill_col(x, OFF); }
  }

  FastLED.show();
  FastLED.delay(26);

  x_wipe_i+=x_wipe_dir;
  if(x_wipe_i >= MATRIX_X + 7){x_wipe_dir = -1; x_wipe_i = MATRIX_X;}
  if(x_wipe_i < -6){x_wipe_dir = 1; x_wipe_i = 0; return true;}
  return false;
}

struct tm timeinfo;
uint8_t h;
int show_time() {
  fill_y_rainbow();
  m_y_count += 3;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }
  
  uint8_t h0 = timeinfo.tm_hour % 10;
  uint8_t h1 = timeinfo.tm_hour / 10;
  uint8_t m0 = timeinfo.tm_min % 10;
  uint8_t m1 = timeinfo.tm_min / 10;
//   uint8_t s0 = timeinfo.tm_sec % 10;
//   uint8_t s1 = timeinfo.tm_sec / 10;

  set_num(0, h1, OFF, true);
  set_num(1, h0, OFF, true);
  set_num(2, m1, OFF, true);
  set_num(3, m0, OFF, true);
  
  h = timeinfo.tm_hour;
  if(h >= 23 && h < 6) FastLED.setBrightness(24);
  else FastLED.setBrightness(128);
  FastLED.show();
  FastLED.delay(25);
  
  if(timeinfo.tm_sec == 59)
  {
    if(timeinfo.tm_min == 59)
      return 3;
    else if((timeinfo.tm_min + 1) % 5 == 0)
      return 2;
    else
      return 1;
  }
  else
  {
    return 0;
  }
}