#include "globals.h"
#include "char.h"

void set_seg(uint8_t chr_i, uint8_t seg_i, CRGB color) {
   fill_solid(leds +(chr_i * LED_PER_SEG * 7) + (seg_i*LED_PER_SEG), LED_PER_SEG, color);
}

void set_char_raw(uint8_t i, byte c, CRGB color, bool transparent) {
  static byte x;
  static CRGB color_val;
  for(x=0; x < 7; x++){
    if(transparent){
      if((c & _BV(x)) == 0){
        set_seg(i, x, OFF);
      }
    }
    else {
      set_seg(i, x, (c & _BV(x)) ? color : OFF);
    }
  }
}

void set_char_raw(uint8_t i, byte c, CRGB color) {
  set_char_raw(i, c, color, false);
}

void set_char(uint8_t i, char c, CRGB color, bool transparent) {
  static byte c_val = 0;

  c_val = byte(c);
  if (c_val < 32 || c_val > 122) { return; } // out of range
  c_val = _chars[c_val - CHAR_OFFSET];

  set_char_raw(i, c_val, color, transparent);
}

void set_char(uint8_t i, char c, CRGB color) {
  set_char(i, c, color, false);
}

void set_num(uint8_t i, uint8_t n, CRGB color, bool transparent) {
    if(n < 0 || n > 9) return; // only single digits please!
    set_char_raw(i, _chars[n + NUM_START_OFFSET], color, transparent);
}

void set_num(uint8_t i, uint8_t n, CRGB color) {
    set_num(i, n, color, false);
}
