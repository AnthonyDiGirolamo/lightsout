class LightsOut {
  public:
    char buffer[20];
    uint8_t current_level;
    uint8_t required_moves;
    uint8_t current_move_count;
    uint16_t current_board;
    uint16_t current_board_reset;
    uint16_t current_board_solution;
    int button;

    LightsOut() {
      randomSeed(RANDOMSEED1);
      current_level = 0;
      advance_level();
    }
    void randomize_board() {
      current_board = (uint16_t) rand();
      current_board_reset = current_board;
    }
    void reset_board() {
      current_move_count = 0;
      current_board = current_board_reset;
    }
    void advance_level() {
      randomize_board();
      current_level++;
      current_move_count = 0;
      current_board_solution = find_solution();
      required_moves = count_ones(current_board_solution);
    }
    bool has_won() {
      return (current_board == 0);
    }
    uint16_t toggle(uint8_t space) {
      current_board ^= space_masks[space];
      return current_board;
    }
    uint16_t push(uint8_t space) {
      current_move_count++;
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
    void update_board() {
      uint16_t lit = 0;
      for(int i=15; i>=0; i--) {
        lit = (current_board & space_masks[i]) >> i;
        strip.setPixelColor(board_light_index[i], (lit ? lights_out_color_schemes[current_scheme] : lights_out_color_schemes[current_scheme+1]));
      }
      strip.show();
    }
    void update_text() {
      max_print_progmem(string_level, 0, 0);
      sprintf(buffer, "%2u", current_level);
      /* sprintf(buffer, "L%2u %2u", current_level, button); */
      alpha_board.write_string(buffer, 0, 5);
      sprintf(buffer, "  %3u/%2u", current_move_count, required_moves);
      alpha_board.write_string(buffer, 1, 0);
    }
    void begin() {
      update_board();
      update_text();

      delay(1000);
      while (1) {
        button = read_buttons();
        if (button >= 0) { // there is a button press
          if (button <= 31 && button >= 16) {
            // long hold, pause menu
            reset_board();
            update_board();
            update_text();
            delay(500);
          }
          else if (button >= 0 && button <= 15) {
            // normal press, update board
            push(button);
            delay(250); // avoid toggling too fast
            update_board();
            update_text();

            if (has_won()) {
              max_print_progmem(string_win, 0, 0);
              delay(5000);
              advance_level();
              update_board();
              update_text();
            }
          }
        }

        /* if (millis()-timer > 4000) { */
        /* } */
      } // end while
    }
};

// vim: ft=cpp

