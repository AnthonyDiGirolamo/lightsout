void print_16_bits(uint16_t n) {
  char s[32];
  sprintf(s, "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d",
    (n & 0x08000 ? 1 : 0), \
    (n & 0x04000 ? 1 : 0), \
    (n & 0x02000 ? 1 : 0), \
    (n & 0x01000 ? 1 : 0), \
    (n & 0x0800 ? 1 : 0), \
    (n & 0x0400 ? 1 : 0), \
    (n & 0x0200 ? 1 : 0), \
    (n & 0x0100 ? 1 : 0), \
    (n & 0x080 ? 1 : 0), \
    (n & 0x040 ? 1 : 0), \
    (n & 0x020 ? 1 : 0), \
    (n & 0x010 ? 1 : 0), \
    (n & 0x08 ? 1 : 0), \
    (n & 0x04 ? 1 : 0), \
    (n & 0x02 ? 1 : 0), \
    (n & 0x01 ? 1 : 0));
  Serial.println(s);
}

void print_4_bits(uint16_t n) {
  char s[16];
  sprintf(s, "%d%d%d%d",
    (n & 0x08 ? 1 : 0), \
    (n & 0x04 ? 1 : 0), \
    (n & 0x02 ? 1 : 0), \
    (n & 0x01 ? 1 : 0));
  Serial.print(s);
}

void print_board(uint16_t board) {
  Serial.print("+------+\n| ");
  print_4_bits( (board & 0xF000) >> 12 );
  Serial.print(" |\n| ");
  print_4_bits( (board &  0xF00) >> 8 );
  Serial.print(" |\n| ");
  print_4_bits( (board &   0xF0) >> 4 );
  Serial.print(" |\n| ");
  print_4_bits( (board &    0xF) );
  Serial.print(" |\n+------+\n");
}

class LightsOut {
  public:
    uint8_t current_level;
    uint8_t required_moves;
    uint8_t current_move_count;
    uint16_t current_board;
    uint16_t current_board_solution;

    LightsOut() {
      randomSeed(31415926);
      current_level = 0;
      advance_level();
    }
    void randomize_board() {
      current_board = (uint16_t) rand();
    }
    void advance_level() {
      randomize_board();
      current_level++;
      current_move_count = 0;
      current_board_solution = find_solution();
      required_moves = count_ones(current_board_solution);
    }
    uint8_t has_won() {
      return (current_board == 0);
    }
    uint16_t toggle(uint8_t space) {
      current_board ^= space_masks[space];
      return current_board;
    }
    uint16_t push(uint8_t space) {
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
};

void max_print(char* string, int col = -1, int row = -1) {
  strcpy_P(buffer, string);
  if (col >= 0 && row >= 0)
    Serial.println(buffer);
    //alpha_board.write_string(buffer, col, row);
  else
    Serial.println(buffer);
    //alpha_board.write_string(buffer);
}

void setup() {
  Serial.begin(9600);
}


void loop() {
  int i = 0;

  // Test pushing buttons
  LightsOut game = LightsOut();
  game.current_board = 65535;
  for(i = 0; i<16; i++) {
    print_board(game.push(i));
  }
  if (game.has_won())

  // Run a few games
  game = LightsOut();
  for(i=0; i<3; i++) {
    max_print(string_level);
    Serial.println(game.current_level);
    max_print(string_board);
    print_16_bits(game.current_board);
    max_print(string_solution);
    print_16_bits(game.current_board_solution);
    max_print(string_moves);
    Serial.println(game.required_moves);
    if (game.has_won())
      max_print(string_win);

    game.advance_level();
  }

  delay(999999);
}

// vim: ft=cpp
