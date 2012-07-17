// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b) {
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  int i, j;

  for (j=0; j < 256 * 10; j++) {     // 10 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(spiral_light_index[i], Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

void max_print_progmem(char* string, uint8_t col = -1, uint8_t row = -1) {
  char buffer[32];
  strcpy_P(buffer, string);
  alpha_board.write_string(buffer,
      col>=0 ? col : 0,
      row>=0 ? row : 0);
}

void print_16_bits(uint16_t n) {
  char s[32];
  sprintf(s, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
    (n & 0x08000 ? 1 : 0), \
    (n & 0x04000 ? 1 : 0), \
    (n & 0x02000 ? 1 : 0), \
    (n & 0x01000 ? 1 : 0), \
    (n & 0x0800 ? 1 : 0), \
    (n & 0x0400 ? 1 : 0), \
    (n & 0x0200 ? 1 : 0), \
    (n & 0x0100 ? 1 : 0), \
    (n & 0x080 ? 1 : 0), \
    (n & 0x040 ? 1 : 0), \
    (n & 0x020 ? 1 : 0), \
    (n & 0x010 ? 1 : 0), \
    (n & 0x08 ? 1 : 0), \
    (n & 0x04 ? 1 : 0), \
    (n & 0x02 ? 1 : 0), \
    (n & 0x01 ? 1 : 0));
  /* Serial.println(s); */
  alpha_board.write_string(s, 0, 0);
  alpha_board.write_string(&s[8], 1, 0);
}

// void print_4_bits(uint16_t n) {
//   char s[16];
//   sprintf(s, "%d%d%d%d",
//     (n & 0x08 ? 1 : 0), \
//     (n & 0x04 ? 1 : 0), \
//     (n & 0x02 ? 1 : 0), \
//     (n & 0x01 ? 1 : 0));
//   Serial.print(s);
// }

// void print_board(uint16_t board) {
//   Serial.print("+------+\n| ");
//   print_4_bits( (board & 0xF000) >> 12 );
//   Serial.print(" |\n| ");
//   print_4_bits( (board &  0xF00) >> 8 );
//   Serial.print(" |\n| ");
//   print_4_bits( (board &   0xF0) >> 4 );
//   Serial.print(" |\n| ");
//   print_4_bits( (board &    0xF) );
//   Serial.print(" |\n+------+\n");
// }

// Return a button press 0-15
// If a button is being held, return button value + 16
int read_buttons(uint8_t skip_delay = 0) {
  static unsigned long button_timer;
  static uint16_t buttons1, buttons2;

  int index = -1;
  buttons1 = mcp.readGPIOAB();
  //print_16_bits(buttons1);
  if ((~buttons1) > 0) { // if there is a button being pressed
    for (int i=0; i<16; i++) {
      if ( (((~buttons1) >> i) & 1) == 1 ) {
        index = i;
        break;
      }
    }
    if (buttons1 != buttons2) { // a different button from last time
      buttons2 = buttons1;
      button_timer = millis();
      if (!skip_delay)
        delay(40); // to prevent button bouncing
    }
    else { // same button as last, check for long press
      if (millis()-button_timer > 2000) {
        index += 16;
      }
    }
  }
  else {
    button_timer = millis();
  }
  return index;
}

void print_time(unsigned long start, unsigned long end, int row = 0, int col = 0) {
  char b[8];
  unsigned long t;
  t = ((end - start) / 1000);
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  /* sprintf(b, "%02u:%02u:%02u:%u", h, m, s, ((end-start)/100)%10); */
  sprintf(b, "%02u.%02u", m, s);
  alpha_board.write_string(b, row, col);
}

// vim: ft=cpp
