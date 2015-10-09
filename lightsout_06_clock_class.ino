class Clock {
  public:
    char buffer[20];
    uint8_t row_mode[2];
    int button;
    DateTime now;
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;

    Clock() {
      max_print_progmem(string_empty, 0, 0);
      max_print_progmem(string_empty, 1, 0);
      row_mode[0] = 0;
      row_mode[1] = 0;
      update_time();
      update_temp();
    }

    void update_time() {
      now     = RTC.now();
      year    = now.year();
      month   = now.month();
      day     = now.day();
      hour    = now.hour();
      minute  = now.minute();
      second  = now.second();
      weekday = now.dayOfWeek();

      /* Serial.print(year, DEC); */
      /* Serial.print('/'); */
      /* Serial.print(month, DEC); */
      /* Serial.print('/'); */
      /* Serial.print(day, DEC); */
      /* Serial.print(' '); */
      /* Serial.print(hour, DEC); */
      /* Serial.print(':'); */
      /* Serial.print(minute, DEC); */
      /* Serial.print(':'); */
      /* Serial.print(second, DEC); */
      /* Serial.println(); */
    }

    void update_temp() {
      // RTC.forceTempConv(true); //DS3231 does this every 64 seconds, we are simply testing the function here
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
      strcpy_P(&buffer[6], (char*)pgm_read_word(&(string_days[weekday])));
      alpha_board.write_string(buffer, row, 0);
    }

    void begin() {
      delay(1000);

      unsigned long time = millis() - MENU_DELAY;
      while (1) {
        if (millis() - time > MENU_DELAY) {
          time = millis();
          update_time();
          display_24hr_time(0);
          /* display_mmddyy_date(1); */
          display_mmdd_weekday_date(1);
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
