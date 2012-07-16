class ColorPicker {
  public:
    char buffer[20];
    bool using_all_lights;
    bool fading;
    uint8_t red, green, blue;
    uint8_t rand_red, rand_green, rand_blue;
    uint8_t button;
    unsigned long timer;
    unsigned long fade_timer;
    int fade_i, fade_j;

    ColorPicker() {
      using_all_lights = false;
      fading = false;
      randomSeed(millis()+(unsigned long) analogRead(3));
      set_control_colors();
      generate_random_color();
      reset_white();
    }

    void set_control_colors() {
      if (!using_all_lights) {
        strip.setPixelColor(board_light_index[7], 0xFF0000);
        strip.setPixelColor(board_light_index[3], 0x010000);
        strip.setPixelColor(board_light_index[6], 0x00FF00);
        strip.setPixelColor(board_light_index[2], 0x000100);
        strip.setPixelColor(board_light_index[5], 0x0000FF);
        strip.setPixelColor(board_light_index[1], 0x000001);
        strip.setPixelColor(board_light_index[0], 0x010101);
        strip.show();
      }
    }

    void generate_random_color() {
      timer = millis();
      rand_red = (uint8_t) rand();
      rand_green = (uint8_t) rand();
      rand_blue = (uint8_t) rand();
      if (!using_all_lights) {
        strip.setPixelColor(board_light_index[4], Color(rand_red, rand_green, rand_blue));
        strip.show();
      }
    }

    void use_random_color() {
      red = rand_red;
      green = rand_green;
      blue = rand_blue;
    }

    void reset_white() {
      red = 1;
      green = 1;
      blue = 1;
    }

    void update_color() {
      if (!fading) {
        uint32_t c = Color(red, green, blue);
        for (int i=15; i>= (using_all_lights ? 0 : 8) ; i--) {
          strip.setPixelColor(board_light_index[i], c);
        }
        strip.show();
      }
    }

    void update_text() {
      if (fading)
        max_print_progmem(string_fade, 0, 0);
      else if (using_all_lights)
        max_print_progmem(string_full, 0, 0);
      else
        max_print_progmem(string_half, 0, 0);

      sprintf(buffer, "0x%02X%02X%02X", red, green, blue);
      // sprintf(buffer, "R%03uG%03uB%03u", red, green, blue);
      alpha_board.write_string(buffer, 1, 0);
    }

    void toggle_fading() {
      fading = fading ? false : true;
      using_all_lights = false;

      if (fading) {
        fade_i = 0; fade_j = 0;
        fade_timer = millis();
      }
      else {
        set_control_colors();
      }
    }

    void toggle_all_lights() {
      if (!fading) {
        using_all_lights = using_all_lights ? false : true;
        set_control_colors();
      }
    }

    void begin() {
      while (1) {
        button = read_buttons();
        if (button >= 0) {
          switch(button) {
            case -1:
              break;
            case 7:
            case 23:
              // more red
              if (red < 255)
                red++;
              break;
            case 3:
            case 19:
              // less red
              if (red > 0)
                red--;
              break;
            case 6:
            case 22:
              // more green
              if (green < 255)
                green++;
              break;
            case 2:
            case 18:
              // less green
              if (green > 0)
                green--;
              break;
            case 5:
            case 21:
              // more blue
              if (blue < 255)
                blue++;
              break;
            case 1:
            case 17:
              // less blue
              if (blue > 0)
                blue--;
              break;
            case 4:
            case 20:
              // use random
              use_random_color();
              generate_random_color();
              break;
            case 0:
            case 16:
              toggle_fading();
              break;
            case 15:
            case 14:
            case 13:
            case 12:
            case 11:
            case 10:
            case 9:
            case 8:
              toggle_all_lights();
              break;
          }
          update_color();
          update_text();

          // extra delay to slow down fast presses
          if (button == 16 || button == 0 || button == 4 || button == 20)
            delay(1000);
          else
            delay(100);
        }

        if (millis()-timer > 3000) {
          generate_random_color();
        }

        if (fading /*&& (millis()-fade_timer > 5)*/) {
          /* fade_timer=millis(); */
          for (fade_i=0; fade_i < strip.numPixels(); fade_i++) {
            // tricky math! we use each pixel as a fraction of the full 96-color wheel
            // (thats the i / strip.numPixels() part)
            // Then add in j which makes the colors go around per pixel
            // the % 96 is to make the wheel cycle around
            strip.setPixelColor(fade_i, Wheel( ((fade_i * 256 / strip.numPixels()) + fade_j) % 256) );
          }
          strip.show();   // write all the pixels out
          fade_j++;
          if (fade_j > 1280)
            fade_j = 0;
        }

      } // end while
    }
};

// vim: ft=cpp
