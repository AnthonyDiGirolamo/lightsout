class LightsOut {
  public:
    char buffer[20];
    bool random_game;
    uint8_t current_level;
    uint8_t current_random_level;
    uint8_t required_moves;
    uint16_t current_move_count;
    uint16_t current_board;
    uint16_t current_board_reset;
    uint16_t current_board_solution;
    uint16_t last_random_level;
    int button;
    unsigned long level_start_time;
    uint32_t lit_color;

    LightsOut() {
      random_game = false;
      randomSeed(RANDOMSEED1);
      current_level = 0;
      current_random_level = 0;
      advance_level();
      EEPROM_readAnything(0, lit_color);
    }

    void randomize_board() {
      current_board = (uint16_t) rand();
      current_board_reset = current_board;
      last_random_level = current_board;
    }

    void reset_board() {
      current_move_count = 0;
      current_board = current_board_reset;
      level_start_time = millis();
    }

    void advance_level() {
      if (!random_game && current_level>31) {
        random_game = true;
        current_random_level = 0;
      }

      if (random_game) {
        current_random_level++;
        randomize_board();
      }
      else {
        current_level++;
        /* memcmp_PF(&current_board, mini_lightsout_levels[current_level], 2); */
        current_board = mini_lightsout_levels[current_level];
        current_board_reset = current_board;
      }

      reset_board();
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

    void display_solution() {
      uint16_t lit = 0;
      for(int i=15; i>=0; i--) {
        lit = (current_board_solution & space_masks[i]) >> i;
        set_strip_with_gamma_correction(board_light_index[i], (lit ? lights_out_color_schemes[current_scheme+2] : lights_out_color_schemes[current_scheme+3]));
      }
      strip.show();
    }

    void update_board() {
      uint16_t lit = 0;
      for(int i=15; i>=0; i--) {
        lit = (current_board & space_masks[i]) >> i;
        set_strip_with_gamma_correction(board_light_index[i], (lit ? lit_color : lights_out_color_schemes[current_scheme+1]));
      }
      strip.show();
    }

    void update_text() {
      /* max_print_progmem(string_level, 0, 0); */
      /* sprintf(buffer, "%02u", current_level); */
      /* alpha_board.write_string(buffer, 0, 6); */

      sprintf(buffer, "%c%02u ", (random_game ? 'R' : 'L'), (random_game ? current_random_level: current_level));
      alpha_board.write_string(buffer, 0, 0);

      print_time(level_start_time, millis(), 0, 4);

      max_print_progmem(string_empty, 1, 0);
      sprintf(buffer, "  %3u/%02u", current_move_count, required_moves);
      alpha_board.write_string(buffer, 1, 0);
    }

    void pause() {
      current_move_count -= 8;

      int i = 1;
      uint8_t done = 0;
      unsigned long pause_start = millis();
      unsigned long time = millis();

      max_print_progmem(string_paused, 0, 0);
      max_print_progmem(string_empty, 1, 0);
      colorWipe(main_menu_color_schemes[current_scheme], 0);
      // Set Dim Colors
      for (int x=0; x<5; x++)
        set_strip_with_gamma_correction(board_light_index[15-x], main_menu_color_schemes_dim[current_scheme+1+x]);
      strip.show();
      delay(1000);

      while (!done) {
        // Display Menu Options
        if ((millis()-time) > MENU_DELAY) {
          time = millis();

          // Display Help Text
          if (i == 1) {
            max_print_progmem(string_restart, 0, 0);
            max_print_progmem(string_empty, 1, 0);
          }
          else if (i == 2) {
            max_print_progmem(string_solution, 0, 0);
            max_print_progmem(string_empty, 1, 0);
          }
          else if (i == 3) {
            max_print_progmem(string_next, 0, 0);
            max_print_progmem(string_level, 1, 0);
          }
          else if (i == 4) {
            max_print_progmem(string_random, 0, 0);
            max_print_progmem(string_level, 1, 0);
          }
          else if (i == 5) {
            max_print_progmem(string_cancel, 0, 0);
            max_print_progmem(string_empty, 1, 0);
          }

          // Set Dim Colors
          for (int x=0; x<5; x++)
            set_strip_with_gamma_correction(board_light_index[15-x], main_menu_color_schemes_dim[current_scheme+1+x]);

          // Highlight Current Menu Option
          set_strip_with_gamma_correction(board_light_index[16-i], main_menu_color_schemes[current_scheme+i]);
          strip.show();

          i++;
          if (i>5)
            i=1;
        }

        // Read Buttons
        button = read_buttons();
        delay(250);

        if (button >= 0) {
          if (button == 15) {
            reset_board();
            done = 1;
          }
          else if (button == 14) {
            max_print_progmem(string_solution, 0, 0);
            max_print_progmem(string_empty, 1, 0);
            display_solution();
            button = -1;
            while (button>31 || button<0)
              button = read_buttons();
            done = 1;
          }
          else if (button == 13) {
            if (random_game) {
              max_print_progmem(string_normal, 0, 0);
              max_print_progmem(string_levels, 1, 0);
              delay(1000);
              random_game = false;

              current_board = mini_lightsout_levels[current_level];
              current_board_reset = current_board;
              reset_board();
              current_board_solution = find_solution();
              required_moves = count_ones(current_board_solution);
            }
            else {
              advance_level();
            }
            done = 1;
          }
          else if (button == 12) {
            if (!random_game) {
              max_print_progmem(string_random, 0, 0);
              max_print_progmem(string_levels, 1, 0);
              delay(1000);
              random_game = true;

              if (current_random_level == 0) {
                current_random_level++;
                randomize_board();
              }
              else {
                current_board = last_random_level;
                current_board_reset = last_random_level;
              }
              reset_board();
              current_board_solution = find_solution();
              required_moves = count_ones(current_board_solution);
            }
            else {
              advance_level();
            }
            done = 1;
          }
          else if (button == 11) {
            done = 1;
          }
        }

      }
      level_start_time = pause_start;
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
            pause();
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
              max_print_progmem(string_you, 0, 0);
              max_print_progmem(string_win, 1, 0);
              /* for (int x=0; x<4; x++) { */
              /*   max_print_progmem(string_win, 0, 0); */
              /*   delay(800); */
              /*   max_print_progmem(string_empty, 0, 0); */
              /*   delay(800); */
              /* } */
              rainbowCycle(1);



              advance_level();
              update_board();
              update_text();
            }
          }
        }

        print_time(level_start_time, millis(), 0, 4);
        /* if (millis()-timer > 4000) { */
        /* } */
      } // end while
    }
};

// vim: ft=cpp
