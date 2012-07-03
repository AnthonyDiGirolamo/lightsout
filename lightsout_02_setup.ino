// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void print_16_bits(uint16_t number) {
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
  Serial.println(s);
}

void update_board(uint16_t) {
  uint16_t lit = 0;
  for(int i=15; i>=0; i--) {
    lit = (number & space_masks[i]) >> i;
    strip.setPixelColor(board_light_index[i], (lit ? lights_out_color_schemes[current_scheme] : lights_out_color_schemes[current_scheme+1]));
  }
  strip.show();
}

void print_4_bits(uint16_t n) {
  char s[16];
  sprintf(s, "%d%d%d%d",
    (n & 0x08 ? 1 : 0), \
    (n & 0x04 ? 1 : 0), \
    (n & 0x02 ? 1 : 0), \
    (n & 0x01 ? 1 : 0));
  Serial.print(s);
}

void print_board(uint16_t board) {
  Serial.print("+------+\n| ");
  print_4_bits( (board & 0xF000) >> 12 );
  Serial.print(" |\n| ");
  print_4_bits( (board &  0xF00) >> 8 );
  Serial.print(" |\n| ");
  print_4_bits( (board &   0xF0) >> 4 );
  Serial.print(" |\n| ");
  print_4_bits( (board &    0xF) );
  Serial.print(" |\n+------+\n");
}

class LightsOut {
  public:
    uint8_t current_level;
    uint8_t required_moves;
    uint8_t current_move_count;
    uint16_t current_board;
    uint16_t current_board_solution;

    LightsOut() {
      randomSeed(RANDOMSEED);
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

//uint16_t read_buttons() {
  ////buttons1 = mcp.readGPIOAB();
  //if (buttons1 != buttons2) {
    //buttons3 = buttons2;
    //buttons2 = buttons1;
    //print_16_bits(buttons1);
    //delay(40);
  //}
  //return buttons1;
//}

void max_print(char* string, int col = -1, int row = -1) {
  char buffer[32];
  strcpy_P(buffer, string);
  if (col >= 0 && row >= 0) {
    //Serial.println(buffer);
    alpha_board.write_string(buffer, col, row);
  }
  else {
    //Serial.println(buffer);
    alpha_board.write_string(buffer, 0, 0);
  }
}


void setup() {
  Serial.begin(9600);
  alpha_board.begin();
  alpha_board.set_global_brightness(5);

  strip.begin();
  strip.show();

  //for(int i=0; i<16; i++){
    //strip.setPixelColor(board_light_index[i], Color(255,0,0));
    //strip.show();
    //delay(1000);
  //}

  max_print(string_testing, 0, 0);
  max_print(string_testing, 1, 0);
  colorWipe(Color(0, 0, 255), 50);
  colorWipe(Color(0, 255, 0), 50);
  colorWipe(Color(255, 0, 0), 50);
  colorWipe(Color(255, 255, 255), 50);

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
        max_print(string_mainmenu, 0, 0);
        max_print(string_empty, 1, 0);
        strip.setPixelColor(board_light_index[15],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }
      else if (i == LIGHTS) {
        max_print(string_lights, 0, 0);
        max_print(string_out, 1, 0);
        strip.setPixelColor(board_light_index[14],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }
      else if (i == MEMORY) {
        max_print(string_color, 0, 0);
        max_print(string_memory, 1, 0);
        strip.setPixelColor(board_light_index[13],
          main_menu_color_schemes[current_scheme+MAINMENU]);
      }
      else if (i == PICKER) {
        max_print(string_color, 0, 0);
        max_print(string_picker, 1, 0);
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
    //max_print(string_win);

  //delay(1000);
  //// Run a few games
  //game = LightsOut();
  //for(i=0; i<1; i++) {
    //max_print(string_level);
    //Serial.println(game.current_level);
    //delay(1000);

    //max_print(string_board);
    //print_16_bits(game.current_board);
    //update_board(game.current_board);
    //delay(10000);

    //max_print(string_solution);
    //print_16_bits(game.current_board_solution);
    //update_board(game.current_board_solution);
    //delay(10000);

    //max_print(string_moves);
    //Serial.println(game.required_moves);
    //delay(10000);

    //if (game.has_won())
      //max_print(string_win);

    //delay(10000);
    //game.advance_level();
  //}

  //delay(999999);
}

// vim: ft=cpp
