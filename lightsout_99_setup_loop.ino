void setup() {
  Serial.begin(9600);

  // Setup 16-segment display
  alpha_board.begin();
  alpha_board.set_global_brightness(4);

  // Setup RGB Strip
  strip.begin();
  strip.show();

  // Setup port expander
  mcp.begin();
  for (int i=0; i<16; i++) {
    mcp.pinMode(i, INPUT);
    mcp.pullUp(i, HIGH);  // turn on a 100K pullup internally
  }

  /* delay(5000); */
  /* for (int i=0; i< 32; i++) { */
  /*   Serial.println(mini_lightsout_levels[i], BIN); */
  /* } */

  main_menu();
}

void main_menu() {
  int i = 1, button = 0;
  unsigned long time = millis();

  max_print_progmem(string_mainmenu1, 0, 0);
  max_print_progmem(string_mainmenu2, 1, 0);
  colorWipe(main_menu_color_schemes[current_scheme], 0);
  // Set Dim Colors
  for (int x=0; x<3; x++)
    strip.setPixelColor(board_light_index[15-x], main_menu_color_schemes_dim[current_scheme+1+x]);
  strip.show();
  delay(1000);

  while (1) {
    // if MENU_DELAY time has elapsed
    if (millis() - time > MENU_DELAY) {
      if (i == 1) {
        max_print_progmem(string_lights, 0, 0);
        max_print_progmem(string_out, 1, 0);
      }
      else if (i == 2) {
        max_print_progmem(string_color, 0, 0);
        max_print_progmem(string_picker, 1, 0);
      }
      else if (i == 3) {
        max_print_progmem(string_calc, 0, 0);
        max_print_progmem(string_empty, 1, 0);
      }

      // Set Dim Colors
      for (int x=0; x<3; x++)
        strip.setPixelColor(board_light_index[15-x], main_menu_color_schemes_dim[current_scheme+1+x]);

      // Highlight Current Menu Option
      strip.setPixelColor(board_light_index[16-i], main_menu_color_schemes[current_scheme+i]);
      strip.show();

      i++;
      if (i>3)
        i=1;
      time = millis();
    }

    // check for button press
    button = read_buttons();
    if (button >= 0) {
      if (button == 15) {
        LightsOut game = LightsOut();
        game.begin();
      }
      else if (button == 14) {
        ColorPicker cp = ColorPicker();
        cp.begin();
      }
      else if (button == 13) {
        Calc cal = Calc();
        cal.begin();
      }
    }

  }
}

void loop() {
}

// vim: ft=cpp
