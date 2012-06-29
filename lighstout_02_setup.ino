void print_16_bits(uint16_t n) {
  char s[16];
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
    uint16_t current_board;

    LightsOut() {
      randomSeed(31415926);
      current_level = 1;
      randomize_board();
    }
    void randomize_board() {
      current_board = (uint16_t) rand();
    }
    void advance_level() {
      randomize_board();
      current_level++;
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
};


void setup() {
  Serial.begin(9600);
}

LightsOut lo = LightsOut();

void loop() {
  int i = 0;

  print_16_bits(lo.current_board);
  lo.advance_level();

  lo.current_board = 65535;

  for(i = 0; i<16; i++) {
    print_board(lo.push(i));
  }

  if (lo.has_won())
    Serial.print("You win!\n");

  //Serial.println(sizeof(solving_matrix[0]));
  //Serial.println(sizeof(solving_matrix));

  lo.current_board = levels[0];
  print_16_bits(lo.current_board);
  print_16_bits(lo.find_solution());

  delay(999999);
}

// vim: ft=cpp
