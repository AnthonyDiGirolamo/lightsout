void setup() {
  /* Serial.begin(9600); */
  /* Serial.begin(57600); */

  // Setup 16-segment display
  alpha_board.begin();
  alpha_board.set_global_brightness(4);

  // Setup RGB Strip
  strip.begin();
  strip.show();

  Wire.begin();
  RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    //Serial.println("RTC is older than compile time!  Updating");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

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

  max_print_progmem(string_lights, 0, 0);
  max_print_progmem(string_out, 1, 0);

  // Test Lowercase (Individual Segment Control)
  //alpha_board.disable_decode_mode();
  //alpha_board.turn_off_individual_segments();
  //alpha_board.individual_segment_test();
  //max_print_progmem(string_lights, 0, 0, 1);
  //max_print_progmem(string_out, 1, 0, 1);
  //alpha_board.write_lowercase_string("Grumpy  wizards ",0,0);
  //delay(2000);
  //alpha_board.write_lowercase_string("make    toxic   ",0,0);
  //delay(2000);
  //alpha_board.write_lowercase_string("brew forthe     ",0,0);
  //delay(2000);
  //alpha_board.write_lowercase_string("evil    Queen   ",0,0);
  //delay(2000);
  //alpha_board.write_lowercase_string("and Jack        ",0,0);
  //delay(2000);
  //alpha_board.enable_decode_mode(); // Enable MAX6954 Built-in Font

  colorWipe(main_menu_color_schemes[current_scheme], 0);
  // Set Dim Colors
  for (int x=0; x<3; x++)
    set_strip_with_gamma_correction(board_light_index[15-x], main_menu_color_schemes_dim[current_scheme+1+x]);
  strip.show();

  // Animation here
  uint32_t lit_color;
  EEPROM_readAnything(0, lit_color);

  // Wipe Upper Left to Lower Right
  for (int x=0; x<7; x++) { print_board(animation1[x],  lit_color); delay(50); }
  for (int x=0; x<7; x++) { print_board(~animation1[x], lit_color); delay(50); }
  print_board(animation1[0], lit_color);

  // Wipe Upper Right to Lower Left
  for (int x=0; x<7; x++) { print_board(animation2[x],  lit_color); delay(50); }
  for (int x=0; x<7; x++) { print_board(~animation2[x], lit_color); delay(50); }
  print_board(animation2[0], lit_color);

  while (1) {
    // if MENU_DELAY time has elapsed
    if (millis() - time > MENU_DELAY) {
      if (i == 1) {
        max_print_progmem(string_clock, 0, 0);
        max_print_progmem(string_empty, 1, 0);
      }
      else if (i == 2) {
        max_print_progmem(string_lights, 0, 0);
        max_print_progmem(string_out, 1, 0);
      }
      else if (i == 3) {
        max_print_progmem(string_color, 0, 0);
        max_print_progmem(string_picker, 1, 0);
      }

      // Set Dim Colors
      for (int x=0; x<3; x++)
        set_strip_with_gamma_correction(board_light_index[15-x], main_menu_color_schemes_dim[current_scheme+1+x]);

      // Highlight Current Menu Option
      set_strip_with_gamma_correction(board_light_index[16-i], main_menu_color_schemes[current_scheme+i]);
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
        Clock clock = Clock();
        clock.begin();
        alpha_board.enable_decode_mode();
      }
      else if (button == 14) {
        LightsOut game = LightsOut();
        game.begin();
      }
      else if (button == 13) {
        ColorPicker cp = ColorPicker();
        cp.begin();
      }
      //else if (button == 13) {
        //Calc cal = Calc();
        //cal.begin();
      //}
    }

  }
}

void loop() {
}

// vim: ft=cpp
