#define CLOCK_FUNCTION_COUNT 9

class Clock {
public:
  char buffer[20];
  int index;

  typedef void (Clock::*clock_member_function)(uint8_t row);
  clock_member_function all_functions[CLOCK_FUNCTION_COUNT];

  uint8_t row_one_mode;
  uint8_t row_two_mode;

  alpha_board_method write;

  DateTime now;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t hour12;
  uint8_t minute;
  uint8_t second;
  uint8_t weekday;

  uint32_t color;
  uint8_t r_index;
  uint8_t g_index;
  uint8_t b_index;

  Clock() {
    colorWipe(Color(0,0,0), 0);
    max_print_progmem(string_empty, 0, 0);
    max_print_progmem(string_empty, 1, 0);

    update_time();
    randomSeed(now.unixtime());

    all_functions[0] = &Clock::display_12hr;
    all_functions[1] = &Clock::display_24hr;
    all_functions[2] = &Clock::display_24hr_with_seconds;
    all_functions[3] = &Clock::display_24hr_with_weekday;
    all_functions[4] = &Clock::display_mmddyy;
    all_functions[5] = &Clock::display_mmdd_with_weekday;
    all_functions[6] = &Clock::display_mmddyyyy;
    all_functions[7] = &Clock::display_hourglass;
    all_functions[8] = &Clock::display_noise;

    row_one_mode = 2;
    row_two_mode = 5;
  }

  void update_time() {
    now     = RTC.now();
    year    = now.year();
    month   = now.month();
    day     = now.day();
    hour    = now.hour();
    hour12  = hour % 12;
    if (hour == 12)
      hour12 = 12;
    minute  = now.minute();
    second  = now.second();
    weekday = now.dayOfWeek();
  }

  int moon_phase(int year, int month, int day) {
    /* Calculates the moon phase (0-7), accurate to 1 segment.
       0 = > new moon.
       4 => Full moon. */
    int g, e;

    if (month == 1) --day;
    else if (month == 2) day += 30;
    else { // m >= 3
      day += 28 + (month-2)*3059/100;
      // adjust for leap years
      if (!(year & 3)) ++day;
      if ((year%100) == 0) --day;
    }

    g = (year-1900)%19 + 1;
    e = (11*g + 18) % 30;
    if ((e == 25 && g > 11) || e == 24) e++;
    return ((((e + day)*6+11)%177)/22 & 7);
  }

  void update_lights() {
    // set all colors to off
    for (index = 0; index < 16; index++)
      strip.setPixelColor(board_light_index[index], 0x000000);

    color = hour > 12 ? hour_colors[1] : hour_colors[0];
    r_index = (uint8_t) ((color >> 16) & 0xFF);
    g_index = (uint8_t) ((color >> 8)  & 0xFF);
    b_index = (uint8_t) ((color)       & 0xFF);

    for (index = 0; index < hour12; index++) {
      strip.setPixelColor(board_light_index[11-index],
                          gamma[ r_index ],
                          gamma[ g_index ],
                          gamma[ b_index ] );
    }
    // strip.setPixelColor(board_light_index[11-hour12],
    //                     gamma[ (r_index/60) * minute ],
    //                     gamma[ (g_index/60) * minute ],
    //                     gamma[ (b_index/60) * minute ] );

    color = hour > 12 ? minute_colors[1] : minute_colors[0];
    r_index = (uint8_t) ((color >> 16) & 0xFF);
    g_index = (uint8_t) ((color >> 8)  & 0xFF);
    b_index = (uint8_t) ((color)       & 0xFF);

    for (index = 0; index < (minute/15); index++) {
      strip.setPixelColor(board_light_index[15-index],
                          gamma[ r_index ],
                          gamma[ g_index ],
                          gamma[ b_index ] );
    }
    strip.setPixelColor(board_light_index[15-(minute/15)],
                        gamma[ (r_index/15) * (minute%15) ],
                        gamma[ (g_index/15) * (minute%15) ],
                        gamma[ (b_index/15) * (minute%15) ] );
    strip.show();
  }

  // void update_temp() {
  //   // RTC.forceTempConv(true); // DS3231 does this every 64 seconds
  //   float temp_float  = RTC.getTempAsFloat();
  //   int16_t temp_word = RTC.getTempAsWord();
  //   int8_t temp_hbyte = temp_word >> 8;
  //   int8_t temp_lbyte = temp_word &= 0x00FF;
  //   // Serial.print("Temp as float: ");
  //   // Serial.print(temp_float, DEC);
  //   // Serial.println();
  //   // Serial.print("Temp as word: ");
  //   // Serial.print(temp_hbyte, DEC);
  //   // Serial.print(".");
  //   // Serial.print(temp_lbyte, DEC);
  //   // Serial.println();
  // }

  void display_12hr(uint8_t row) {
    sprintf(buffer, "%2u.%02u  %cm", hour12, minute, hour > 12 ? 'p' : 'a');
    alpha_board.write_string(buffer, row, 0);
  }

  void display_24hr(uint8_t row) {
    sprintf(buffer, "  %02u.%02u  ", hour, minute);
    alpha_board.write_string(buffer, row, 0);
  }

  void display_24hr_with_seconds(uint8_t row) {
    sprintf(buffer, " %02u.%02u.%02u ", hour, minute, second);
    alpha_board.write_string(buffer, row, 0);
  }

  void display_24hr_with_weekday(uint8_t row) {
    sprintf(buffer, "%02u.%02u    ", hour, minute);
    strcpy_P(&buffer[6], (char*)pgm_read_word( &(string_days[weekday]) ));
    alpha_board.write_string(buffer, row, 0);
  }

  void display_mmddyy(uint8_t row) {
    sprintf(buffer, "%02u/%02u/%02u", month, day, year % 1000);
    alpha_board.write_string(buffer, row, 0);
  }

  void display_mmddyyyy(uint8_t row) {
    sprintf(buffer, "%02u.%02u.%04u", month, day, year);
    alpha_board.write_string(buffer, row, 0);
  }

  void display_mmdd_with_weekday(uint8_t row) {
    sprintf(buffer, "%02u.%02u    ", month, day);
    strcpy_P(&buffer[6], (char*)pgm_read_word( &(string_days[weekday]) ));
    alpha_board.write_string(buffer, row, 0);
  }

  void enable_decode_mode(uint8_t row) {
    write = row == 0 ? &MAX6954::write_chip1 : &MAX6954::write_chip2;
    (alpha_board.*write)(0x01, B11111111);
  }

  void display_hourglass(uint8_t row) {
    write = row == 0 ? &MAX6954::write_chip1 : &MAX6954::write_chip2;
    (alpha_board.*write)(0x01, B00000000);

    uint8_t frame = 0;
    uint8_t start;
    uint8_t stop;
    if (minute % 2 == 0) { // odd
      start = 0;
      stop = 4;
      for(index=4; index<8; index++) {
        (alpha_board.*write)(0x20+index, B00000000);
        (alpha_board.*write)(0x28+index, B00000000);
      }
    } else {
      start = 4;
      stop = 8;
      for(index=0; index<4; index++) {
        (alpha_board.*write)(0x20+index, B10111111);
        (alpha_board.*write)(0x28+index, B11111111);
      }
    }

    for(index=start; index<stop; index++) {
      if (second/15 > index%4) {
        (alpha_board.*write)(0x20+index, B10111111);
        (alpha_board.*write)(0x28+index, B11111111);
      }
      else if (second/15 == index%4) {
        (alpha_board.*write)(0x20+index, B00000000);
        (alpha_board.*write)(0x28+index, B00000000);
        for (frame=0; frame<second%15; frame++) {
          (alpha_board.*write)(0x20+index+animation_hourglass[frame][0], animation_hourglass[frame][1]);
        }
      }
      else {
        (alpha_board.*write)(0x20+index, B00000000);
        (alpha_board.*write)(0x28+index, B00000000);
      }
    }
  }

  void display_noise(uint8_t row) {
    write = row == 0 ? &MAX6954::write_chip1 : &MAX6954::write_chip2;
    (alpha_board.*write)(0x01, B00000000);
    for (index=0x20; index<=0x2F; index++) {
      (alpha_board.*write)(index, (uint8_t) random(0, 256));
    }
  }

  void begin() {

    unsigned long time = millis() - 1000;
    while (1) {
      // update the clock if one second has passed
      if (millis() - time > 1000) {
        time = millis();
        update_time();
        update_lights();
        (this->*all_functions[row_one_mode])(0);
        (this->*all_functions[row_two_mode])(1);
      }

      // Serial.print(" since midnight 1/1/1970 = ");
      // Serial.print(now.unixtime());
      // Serial.print("s = ");
      // Serial.print(now.unixtime() / 86400L);
      // Serial.println("d");

      // // calculate a date which is 7 days and 30 seconds into the future
      // DateTime future (now.unixtime() + 7 * 86400L + 30);
      // Serial.print(" now + 7d + 30s: ");
      // Serial.print(future.year(), DEC);
      // Serial.print('/');
      // Serial.print(future.month(), DEC);
      // Serial.print('/');
      // Serial.print(future.day(), DEC);
      // Serial.print(' ');
      // Serial.print(future.hour(), DEC);
      // Serial.print(':');
      // Serial.print(future.minute(), DEC);
      // Serial.print(':');
      // Serial.print(future.second(), DEC);
      // Serial.println();

      // button = read_buttons();
      check_switches();

      if (justreleased[3])
        break;

      if (justpressed[15]) {
        row_one_mode = (row_one_mode + 1) % CLOCK_FUNCTION_COUNT;
        if (row_one_mode == 0)
          enable_decode_mode(0);
      }
      if (justpressed[11]) {
        row_two_mode = (row_two_mode + 1) % CLOCK_FUNCTION_COUNT;
        if (row_two_mode == 0)
          enable_decode_mode(1);
      }

      // for (index=0; index<NUMBUTTONS; index++) {
      //   if (justpressed[index]) {
      //     Serial.print(index, DEC);
      //     Serial.println(" Just pressed");
      //   }
      //   if (justreleased[index]) {
      //     Serial.print(index, DEC);
      //     Serial.println(" Just released");
      //   }
      //   if (currently_pressed[index]) {
      //     Serial.print(index, DEC);
      //     Serial.println(" currently pressed");
      //   }
      // }

    } // end while
  }
};

// vim: ft=cpp
