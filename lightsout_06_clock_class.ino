class Clock {
  public:
    char buffer[20];
    int button;
    int index;

    typedef void (Clock::*clock_member_function)(uint8_t row);
    clock_member_function row_function[2];

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
      update_temp();
      row_function[0] = &Clock::display_24hr_time;
      /* row_function[1] = &Clock::display_mmdd_weekday_date; */
      row_function[1] = &Clock::display_hourglass;
      individual_segment_mode(1);
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

    void update_lights() {
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
      strip.setPixelColor(board_light_index[11-hour12],
                          gamma[ (r_index/60) * minute ],
                          gamma[ (g_index/60) * minute ],
                          gamma[ (b_index/60) * minute ] );

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

    void update_temp() {
      // RTC.forceTempConv(true); // DS3231 does this every 64 seconds
      float temp_float  = RTC.getTempAsFloat();
      int16_t temp_word = RTC.getTempAsWord();
      int8_t temp_hbyte = temp_word >> 8;
      int8_t temp_lbyte = temp_word &= 0x00FF;

      /* Serial.print("Temp as float: "); */
      /* Serial.print(temp_float, DEC); */
      /* Serial.println(); */
      /* Serial.print("Temp as word: "); */
      /* Serial.print(temp_hbyte, DEC); */
      /* Serial.print("."); */
      /* Serial.print(temp_lbyte, DEC); */
      /* Serial.println(); */
    }

    void display_12hr_time(uint8_t row) {
      sprintf(buffer, "%2u.%02u  %cm", hour12, minute, hour > 12 ? 'p' : 'a');
      alpha_board.write_string(buffer, row, 0);
    }

    void display_24hr_time(uint8_t row) {
      sprintf(buffer, " %02u.%02u.%02u ", hour, minute, second);
      alpha_board.write_string(buffer, row, 0);
    }

    void display_mmddyyyy_date(uint8_t row) {
      sprintf(buffer, "%02u.%02u.%04u", month, day, year);
      alpha_board.write_string(buffer, row, 0);
    }

    void display_mmddyy_date(uint8_t row) {
      sprintf(buffer, "%02u/%02u/%02u", month, day, year % 1000);
      alpha_board.write_string(buffer, row, 0);
    }

    void display_mmdd_weekday_date(uint8_t row) {
      sprintf(buffer, "%02u.%02u    ", month, day);
      strcpy_P(&buffer[6], (char*)pgm_read_word( &(string_days[weekday]) ));
      alpha_board.write_string(buffer, row, 0);
    }

    void individual_segment_mode(uint8_t row) {
      write = row == 0 ? &MAX6954::write_chip1 : &MAX6954::write_chip2;
      // Decode mode enabled
      // alpha_board.write_chip1(0x01, B11111111);
      (alpha_board.*write)(0x01, B00000000);

      // Turn off all segments
      for (int i=0x20; i<=0x2F; i++) {
        (alpha_board.*write)(i, B00000000);
      }
    }

    void display_hourglass(uint8_t row) {
      write = row == 0 ? &MAX6954::write_chip1 : &MAX6954::write_chip2;


      uint8_t frame = 0;
      uint8_t current_second = 0;
      uint8_t start = 4;
      uint8_t stop = 8;
      if (minute % 2 == 0) { // even
        start = 0;
        stop = 4;
      }

      for(index=start; index<stop; index++) {
        if (second/15 > index) {
          (alpha_board.*write)(0x20+index, B10111111);
          (alpha_board.*write)(0x28+index, B11111111);
        }
        else if (second/15 == index) {
          for (frame=0; frame<second%15; frame++) {
            (alpha_board.*write)(0x20+index+animation_hourglass[frame][0], animation_hourglass[frame][1]);
          }
          for (frame=second%15; frame<16; frame++) {
            (alpha_board.*write)(0x20+index+animation_hourglass[frame][0], animation_hourglass[frame][1]);
          }
        }
        else {
          (alpha_board.*write)(0x20+index, B00000000);
          (alpha_board.*write)(0x28+index, B00000000);
        }
        /* (alpha_board.*write)(index, B01111111); */
      }

      /* for(index=0x20; index<0x28; index++) { */
      /*   // Turn on all segments of digit i */
      /*   (alpha_board.*write)(index, B01111111); */
      /*   (alpha_board.*write)(index+8, B01111111); */
      /* } */

      /* for (i=0x20; i<=0x2F; i++) { */
      /*   (alpha_board.*write)(i, (uint8_t) random(0, 256)); */
      /* } */
    }

    void begin() {
      delay(1000);

      unsigned long time = millis() - MENU_DELAY;
      while (1) {
        if (millis() - time > MENU_DELAY) {
          time = millis();
          update_time();
          /* update_lights(); */
          (this->*row_function[0])(0);
          (this->*row_function[1])(1);
        }

        /* Serial.print(" since midnight 1/1/1970 = "); */
        /* Serial.print(now.unixtime()); */
        /* Serial.print("s = "); */
        /* Serial.print(now.unixtime() / 86400L); */
        /* Serial.println("d"); */

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

        button = read_buttons();

        if (button <= 31 && button >= 0) {
          break;
          switch(button) {
            case 7:
            case 23:
              // more red
              /* if (red < 255) */
                /* red++; */
              break;
            case 3:
            case 19:
              // less red
              /* if (red > 0) */
                /* red--; */
              break;
            case 6:
            case 22:
              // more green
              /* if (green < 255) */
                /* green++; */
              break;
            case 2:
            case 18:
              // less green
              /* if (green > 0) */
                /* green--; */
              break;
            case 5:
            case 21:
              // more blue
              /* if (blue < 255) */
                /* blue++; */
              break;
            case 1:
            case 17:
              // less blue
              /* if (blue > 0) */
                /* blue--; */
              break;
            case 4:
            case 20:
              // use random
              /* use_random_color(); */
              /* generate_random_color(); */
              break;
            case 0:
            case 16:
              /* toggle_fading(); */
              break;
            case 15:
            case 14:
            case 13:
            case 12:
            case 11:
            case 10:
            case 9:
            case 8:
              /* toggle_all_lights(); */
              break;
            case 28:
              /* EEPROM_writeAnything(0, Color(red, green, blue)); */
              /* max_print_progmem(string_saved, 0, 0); */
              /* delay(500); */
              break;
          }
          // extra delay to slow down fast presses
          if (button == 16 || button == 0 || button == 4 || button == 20)
            delay(250);
          else
            delay(100);
        }
      } // end while
    }
};

// vim: ft=cpp
