void setup() {
  /* Serial.begin(9600); */

  // Setup 16-segment display
  alpha_board.begin();
  alpha_board.set_global_brightness(5);

  // Setup RGB Strip
  strip.begin();
  strip.show();

  // Setup port expander
  mcp.begin();
  for (int i=0; i<16; i++) {
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }

  main_menu();
}

void main_menu() {
  int i = 0, button = 0;
  unsigned long time = millis();
  while (1) {
    // check for button press and set mode
    button = read_buttons();
    if (button >= 0) {
      if (button == 15) {
        mode = LIGHTS;
        LightsOut game = LightsOut();
        game.begin();
      }
      else if (button == 14) {
        mode = PICKER;
        ColorPicker cp = ColorPicker();
        cp.begin();
      }
    }

    // if MENU_DELAY time has elapsed
    if (millis() - time > MENU_DELAY) {
      // clear colors
      colorWipe(main_menu_color_schemes[current_scheme], 0);

      if (i == MAINMENU) {
        max_print_progmem(string_mainmenu, 0, 0);
        max_print_progmem(string_empty, 1, 0);
        colorWipe(main_menu_color_schemes[current_scheme], 0);
      }
      else if (i == LIGHTS) {
        max_print_progmem(string_lights, 0, 0);
        max_print_progmem(string_out, 1, 0);
        strip.setPixelColor(board_light_index[15],
          main_menu_color_schemes[current_scheme+i]);
        strip.show();
      }
      else if (i == PICKER) {
        max_print_progmem(string_color, 0, 0);
        max_print_progmem(string_picker, 1, 0);
        strip.setPixelColor(board_light_index[14],
          main_menu_color_schemes[current_scheme+i]);
        strip.show();
      }

      i++;
      if (i>PICKER)
        i=LIGHTS;
      time = millis();
    }
  }
}

void loop() {
}

// vim: ft=cpp
