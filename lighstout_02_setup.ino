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
    LightsOut() {
    }
};

uint16_t find_solution(uint16_t level) {
  uint16_t a, b, x, solution;
  solution = 0;
  for(int r=0; r<16; r++) {
    x = 0;
    for(int c=15; c>=0; c--) {
      a = (solving_matrix[r] & space_masks[c]) >> (c); // Unpack matrix value
      b = (level & space_masks[c]) >> (c); // Unpack level value
      x += (a*b);
    }
    x %= 2;
    solution |= (x << (15-r));  // Pack
  }
  print_16_bits(solution);
  return solution;
}


uint16_t toggle(uint16_t board, uint16_t space) {
  return board ^ space_masks[space];
}

uint16_t push(uint16_t board, uint16_t space) {
  uint16_t b = board;
  b ^= space_masks[space];
  b ^= space_masks[neighbors[space][0]];
  b ^= space_masks[neighbors[space][1]];
  b ^= space_masks[neighbors[space][2]];
  b ^= space_masks[neighbors[space][3]];
  return b;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t b;

  b = 65535;
  print_board(b);
  for(int i = 0; i<16; i++)
    b = toggle(b, i);
  print_board(b);

  for(int i = 0; i<16; i++){
    print_board(push(b, i));
  }

  if (b == 0);
    Serial.print("You win!\n");

  randomSeed(3);
  print_16_bits( (uint16_t) rand() );
  print_16_bits( (uint16_t) rand() );
  print_16_bits( (uint16_t) rand() );
  print_16_bits( (uint16_t) rand() );
  print_16_bits( (uint16_t) rand() );

  Serial.println(sizeof(solving_matrix[0]));
  Serial.println(sizeof(solving_matrix));

  print_16_bits(levels[0]);
  find_solution(levels[0]);

  delay(999999);
}

// vim: ft=cpp
