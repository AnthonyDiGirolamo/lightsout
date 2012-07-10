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

void update_board(uint16_t number) {
  uint16_t lit = 0;
  for(int i=15; i>=0; i--) {
    lit = (number & space_masks[i]) >> i;
    strip.setPixelColor(board_light_index[i], (lit ? lights_out_color_schemes[current_scheme] : lights_out_color_schemes[current_scheme+1]));
  }
  strip.show();
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

class ColorPicker {
  public:
    char buffer[16];
    bool using_all_lights;
    bool fading;
    uint8_t red, green, blue;
    uint8_t rand_red, rand_green, rand_blue;
    uint8_t button;
    unsigned long timer;
    unsigned long fade_timer;
    int fade_i, fade_j;

    ColorPicker() {
      using_all_lights = false;
      fading = false;
      randomSeed(RANDOMSEED2);
      set_control_colors();
      generate_random_color();
      reset_white();
    }

    void set_control_colors() {
      if (!using_all_lights) {
        strip.setPixelColor(board_light_index[7], 0xFF0000);
        strip.setPixelColor(board_light_index[3], 0x010000);
        strip.setPixelColor(board_light_index[6], 0x00FF00);
        strip.setPixelColor(board_light_index[2], 0x000100);
        strip.setPixelColor(board_light_index[5], 0x0000FF);
        strip.setPixelColor(board_light_index[1], 0x000001);
        strip.setPixelColor(board_light_index[0], 0x010101);
        strip.show();
      }
    }

    void generate_random_color() {
      timer = millis();
      rand_red = (uint8_t) rand();
      rand_green = (uint8_t) rand();
      rand_blue = (uint8_t) rand();
      if (!using_all_lights) {
        strip.setPixelColor(board_light_index[4], Color(rand_red, rand_green, rand_blue));
        strip.show();
      }
    }

    void use_random_color() {
      red = rand_red;
      green = rand_green;
      blue = rand_blue;
    }

    void reset_white() {
      red = 1;
      green = 1;
      blue = 1;
    }

    void update_color() {
      if (!fading) {
        uint32_t c = Color(red, green, blue);
        for (int i=15; i>7; i--) {
          strip.setPixelColor(board_light_index[i], c);
        }
        strip.show();
      }
    }

    void update_text() {
      max_print_progmem(string_picker);
      sprintf(buffer, " #%02X%02X%02X", red, green, blue);
      // sprintf(buffer, "R%03uG%03uB%03u", red, green, blue);
      alpha_board.write_string(buffer, 1, 0);
    }

    void toggle_fading() {
      fading = fading ? false : true;
      if (fading) {
        using_all_lights = true;
        fade_i = 0; fade_j = 0;
        fade_timer = millis();
      }
      else {
        using_all_lights = false;
        set_control_colors();
      }
    }

    void toggle_all_lights() {
      if (!fading) {
        using_all_lights = using_all_lights ? false : true;
        set_control_colors();
      }
    }

    void begin() {
      while (1) {
        button = read_buttons();
        if (button >= 0) {
          switch(button) {
            case 7:
            case 23:
              // more red
              if (red < 255)
                red++;
              break;
            case 3:
            case 19:
              // less red
              if (red > 0)
                red--;
              break;
            case 6:
            case 22:
              // more green
              if (green < 255)
                green++;
              break;
            case 2:
            case 18:
              // less green
              if (green > 0)
                green--;
              break;
            case 5:
            case 21:
              // more blue
              if (blue < 255)
                blue++;
              break;
            case 1:
            case 17:
              // less blue
              if (blue > 0)
                blue--;
              break;
            case 4:
            case 20:
              // use random
              use_random_color();
              generate_random_color();
              break;
            case 0:
            case 16:
              toggle_fading();
              break;
            case 15:
            case 14:
            case 13:
            case 12:
            case 11:
            case 10:
            case 9:
            case 8:
              toggle_all_lights();
              break;
          }
          update_color();
          update_text();
          if (button == 4 || button == 20)
            delay(1000);
          else
            delay(250);
        }

        if (millis()-timer > 4000) {
          generate_random_color();
        }

        if (fading && (millis()-fade_timer > 20)) {
          fade_timer=millis();
          for (fade_i=0; fade_i < strip.numPixels(); fade_i++) {
            // tricky math! we use each pixel as a fraction of the full 96-color wheel
            // (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 96 is to make the wheel cycle around
            strip.setPixelColor(fade_i, Wheel( ((fade_i * 256 / strip.numPixels()) + fade_j) % 256) );
          }
          strip.show();   // write all the pixels out
          fade_j++;
          if (fade_j > 1280)
            fade_j = 0;
        }

      } // end while
    }
};

class LightsOut {
  public:
    uint8_t current_level;
    uint8_t required_moves;
    uint8_t current_move_count;
    uint16_t current_board;
    uint16_t current_board_solution;

    LightsOut() {
      randomSeed(RANDOMSEED1);
      current_level = 0;
      advance_level();
    }
    void randomize_board() {
      current_board = (uint16_t) rand();
    }
    void advance_level() {
      randomize_board();
      current_level++;
      current_move_count = 0;
      current_board_solution = find_solution();
      required_moves = count_ones(current_board_solution);
    }
    uint8_t has_won() {
      return (current_board == 0);
    }
    uint16_t toggle(uint8_t space) {
      current_board ^= space_masks[space];
      return current_board;
    }
    uint16_t push(uint8_t space) {
      current_board ^= space_masks[space];
      current_board ^= space_masks[neighbors[space][0]];
      current_board ^= space_masks[neighbors[space][1]];
      current_board ^= space_masks[neighbors[space][2]];
      current_board ^= space_masks[neighbors[space][3]];
      return current_board;
    }
    uint16_t find_solution() {
      uint16_t a, b, x, solution;
      solution = 0;
      for(int r=0; r<16; r++) {
        x = 0;
        for(int c=15; c>=0; c--) {
          a = (solving_matrix[r] & space_masks[c]) >> (c); // Unpack matrix value
          b = (current_board & space_masks[c]) >> (c); // Unpack level value
          x += (a*b);
        }
        x %= 2;
        solution |= (x << (15-r));  // Pack
      }
      return solution;
    }
    unsigned int count_ones(unsigned int number) {
      unsigned int v = number; // count the number of bits set in v
      unsigned int c = 0; // c accumulates the total bits set in v
      for (c = 0; v; c++) {
        v &= v - 1; // clear the least significant bit set
      }
      //for (c = 0; v; v >>= 1) {
          //c += v & 1;
      //}
      return c;
    }
};

// Return a button press 0-15
// If a button is being held, return button value + 16
int read_buttons() {
  int index = -1;
  buttons1 = mcp.readGPIOAB();
  //print_16_bits(buttons1);
  if ((~buttons1) > 0) { // if there is a button being pressed
    for (int i=0; i<16; i++) {
      if ( ((~buttons1) >> i) & 1 == 1 ) {
        index = i;
        break;
      }
    }
    if (buttons1 != buttons2) { // a different button from last time
      buttons2 = buttons1;
      button_timer = millis();
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

void setup() {
  /* Serial.begin(9600); */

  // Setup 16-segment display
  alpha_board.begin();
  alpha_board.set_global_brightness(1);

  // Setup RGB Strip
  strip.begin();
  strip.show();

  // Setup port expander
  mcp.begin();
  for (int i=0; i<16; i++) {
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }

  //for(int i=0; i<16; i++){
    //strip.setPixelColor(board_light_index[i], Color(255,0,0));
    //strip.show();
    //delay(1000);
  //}

  //max_print_progmem(string_testing, 0, 0);
  //max_print_progmem(string_testing, 1, 0);
  //colorWipe(Color(0, 0, 255), 50);
  //colorWipe(Color(0, 255, 0), 50);
  //colorWipe(Color(255, 0, 0), 50);
  //colorWipe(Color(255, 255, 255), 50);

  ColorPicker cp = ColorPicker();
  cp.begin();
}

void main_menu() {
  int i = 0;
  unsigned long time = millis();
  uint8_t new_mode = 255;
  while (new_mode != mode) {
    // check for button press and set mode

    // if MENU_DELAY time has elapsed
    if (millis() - time > MENU_DELAY) {
      // clear colors
      colorWipe(main_menu_color_schemes[current_scheme], 0);

      if (i == MAINMENU) {
        max_print_progmem(string_mainmenu, 0, 0);
        max_print_progmem(string_empty, 1, 0);
        strip.setPixelColor(board_light_index[15],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }
      else if (i == LIGHTS) {
        max_print_progmem(string_lights, 0, 0);
        max_print_progmem(string_out, 1, 0);
        strip.setPixelColor(board_light_index[14],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }
      else if (i == MEMORY) {
        max_print_progmem(string_color, 0, 0);
        max_print_progmem(string_memory, 1, 0);
        strip.setPixelColor(board_light_index[13],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }
      else if (i == PICKER) {
        max_print_progmem(string_color, 0, 0);
        max_print_progmem(string_picker, 1, 0);
        strip.setPixelColor(board_light_index[12],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }

      i++;
      if (i>PICKER)
        i=MAINMENU;
      time = millis();
    }
  }
}

void loop() {
  //int i = 0;

  //// Test pushing buttons
  //LightsOut game = LightsOut();
  //game.current_board = 65535;
  //print_16_bits(game.current_board);
  //delay(2000);
  //for(i = 0; i<16; i++) {
    //print_board(game.push(i));
    //print_16_bits(game.current_board);
    //delay(2000);
  //}
  //if (game.has_won())
    //max_print_progmem(string_win);

  //delay(1000);
  //// Run a few games
  //game = LightsOut();
  //for(i=0; i<1; i++) {
    //max_print_progmem(string_level);
    //Serial.println(game.current_level);
    //delay(1000);

    //max_print_progmem(string_board);
    //print_16_bits(game.current_board);
    //update_board(game.current_board);
    //delay(10000);

    //max_print_progmem(string_solution);
    //print_16_bits(game.current_board_solution);
    //update_board(game.current_board_solution);
    //delay(10000);

    //max_print_progmem(string_moves);
    //Serial.println(game.required_moves);
    //delay(10000);

    //if (game.has_won())
      //max_print_progmem(string_win);

    //delay(10000);
    //game.advance_level();
  //}

  //delay(999999);
}

// vim: ft=cpp
