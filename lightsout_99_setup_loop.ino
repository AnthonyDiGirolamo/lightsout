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
