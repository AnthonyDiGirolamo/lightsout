void setup() {
  /* Serial.begin(9600); */

  // Setup 16-segment display
  alpha_board.begin();
  alpha_board.set_global_brightness(2);

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

  main_menu();
}

void main_menu() {
  int i = 0;
  int button;
  unsigned long time = millis();
  uint8_t new_mode = MAINMENU;
  while (1) {
    // check for button press and set mode
    button = read_buttons();
    if (button >= 0) {
      switch(button) {
        case 15:
          new_mode = LIGHTS;
          LightsOut game = LightsOut();
          game.begin();
          break;
        case 14:
          new_mode = PICKER;
          ColorPicker cp = ColorPicker();
          cp.begin();
          break;
      }
    }

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
}

// vim: ft=cpp
