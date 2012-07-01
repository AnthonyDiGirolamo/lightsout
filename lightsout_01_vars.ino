char string_win[]      PROGMEM = "You Win ";
char string_perfect[]  PROGMEM = "Perfect ";
char string_level[]    PROGMEM = "Level   ";
char string_board[]    PROGMEM = "Board   ";
char string_solution[] PROGMEM = "Solution";
char string_moves[]    PROGMEM = "Moves   ";
char string_testing[]  PROGMEM = "TESTING ";

Adafruit_WS2801 strip = Adafruit_WS2801(16, STRIP_DATA, STRIP_CLK);

MAX6954 alpha_board = MAX6954(DATAOUT, DATAIN, CLK, CS1, CS2);

Adafruit_MCP23017 mcp;

prog_uint16_t space_masks[] = {
  0x1,
  0x2,
  0x4,
  0x8,
  0x10,
  0x20,
  0x40,
  0x80,
  0x100,
  0x200,
  0x400,
  0x800,
  0x1000,
  0x2000,
  0x4000,
  0x8000
};

prog_uint8_t neighbors[16][4] = {
// N   S   E   W
  {4,  12, 3,  1},
  {5,  13, 0,  2},
  {6,  14, 1,  3},
  {7,  15, 2,  0},
  {8,  0,  7,  5},
  {9,  1,  4,  6},
  {10, 2,  5,  7},
  {11, 3,  6,  4},
  {12, 4,  11, 9},
  {13, 5,  8,  10},
  {14, 6,  9,  11},
  {15, 7,  10, 8},
  {0,  8,  15, 13},
  {1,  9,  12, 14},
  {2,  10, 13, 15},
  {3,  11, 14, 12}
};

prog_uint8_t board_light_index[] = {
  15,
  14,
  13,
  12,
  8,
  9,
  10,
  11,
  7,
  6,
  5,
  4,
  0,
  1,
  2,
  3
};


//prog_uint16_t uint16_t levels[] = {
/*
obase=16;ibase=2;
1001000111001111
1111111111111111
obase=2;ibase=16;
91CF
FFFF
*/
//}
/*
prog_uint16_t levels[] = {
  0x2687,
  0x6688,
  0x8B91,
  0xACA0,
};
*/
prog_uint16_t solving_matrix[] = {
  0xD808,
  0xE404,
  0x7202,
  0xB101,
  0x8D80,
  0x4E40,
  0x2720,
  0x1B10,
  0x8D8,
  0x4E4,
  0x272,
  0x1B1,
  0x808D,
  0x404E,
  0x2027,
  0x101B
};

/*
prog_uint16_t uint16_t matrix_a[16][16] = {
  {1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0},
  {1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0},
  {0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0},
  {1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1},
  {1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
  {0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0},
  {0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0},
  {0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0},
  {0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0},
  {0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0},
  {0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0},
  {0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1},
  {0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0},
  {0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1},
  {0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,1} };
*/

// vim: ft=cpp
