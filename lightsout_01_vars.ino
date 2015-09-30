// Components Setup

#define STRIP_CLK  6
#define STRIP_DATA 5
#define CS1        9  // MAX6954 1
#define CS2        8  // MAX6954 2
#define DATAOUT    11 // MOSI
#define DATAIN     12 // MISO
#define CLK        13 // sck

Adafruit_WS2801 strip = Adafruit_WS2801(16, STRIP_DATA, STRIP_CLK);

MAX6954 alpha_board = MAX6954(DATAOUT, DATAIN, CLK, CS1, CS2);

Adafruit_MCP23017 mcp;

// Number Constants

#define RANDOMSEED1 31415926ul
#define RANDOMSEED2 62831852ul

// Strings

char string_empty[]    PROGMEM = "                ";

// Main Menu
char string_mainmenu1[] PROGMEM = "Main    ";
char string_mainmenu2[] PROGMEM = "Menu    ";
char string_lights[]   PROGMEM = "Lights  ";
char string_out[]      PROGMEM = "Out     ";
char string_color[]    PROGMEM = "Color   ";
char string_picker[]   PROGMEM = "Chooser ";
char string_calc[]     PROGMEM = "Calc    ";

// Lights Out
char string_paused[]   PROGMEM = "Paused  ";
char string_you[]      PROGMEM = "You";
char string_win[]      PROGMEM = "Win";
char string_perfect[]  PROGMEM = "Perfect ";
char string_level[]    PROGMEM = "Level   ";
char string_levels[]   PROGMEM = "Levels  ";
char string_board[]    PROGMEM = "Board   ";
char string_solution[] PROGMEM = "Solution";
char string_restart[]  PROGMEM = "Restart ";
char string_next[]     PROGMEM = "Next    ";
char string_prev[]     PROGMEM = "Previous";
char string_moves[]    PROGMEM = "Moves   ";
char string_random[]   PROGMEM = "Random  ";
char string_normal[]   PROGMEM = "Normal  ";
char string_cancel[]   PROGMEM = "Resume  ";

// Color Chooser
char string_fade[]     PROGMEM = "Rainbow ";
char string_full[]     PROGMEM = "Full    ";
char string_half[]     PROGMEM = "Half    ";
char string_saved[]    PROGMEM = "Saved   ";

#define MENU_DELAY 1000

// Color Schemes
prog_uint32_t main_menu_color_schemes[] = {
  0x000000, // off
  0xFF9000, // item 1
  0x0000FF, // item 2
  0xFF0000, // item 3
  0x00FF00, // item 4
  0xFF00FF, // item 5
};

prog_uint32_t main_menu_color_schemes_dim[] = {
  0x000000, // off
  0x401800, // item 1
  0x000040, // item 2
  0x400000, // item 3
  0x004000, // item 4
  0x400040, // item 5
};

prog_uint32_t lights_out_color_schemes[] = {
  0xFF9000, // on
  0x000000, // off
  0x000080, // on
  0x000000, // off
};
uint8_t current_scheme = 0; // index of on color in color_schemes array

// Lookup Tables

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

// 15 14 13 12
// 11 10 9  8
// 7  6  5  4
// 3  2  1  0

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

// Strand Board Indexes
// 15 14 13 12
// 11 10 9  8
// 7  6  5  4
// 3  2  1  0

// Strand Hardware Indexes
// 3  2  1  0
// 4  5  6  7
// 11 10 9  8
// 12 13 14 15

// Strand Spiral Indexes
// 3  2  1  0
// 4  13 12 11
// 5  14 15 10
// 6  7  8  9

prog_uint8_t spiral_light_index[] = {
  0,
  1,
  2,
  3,
  4,
  11,
  12,
  13,
  14,
  15,
  8,
  7,
  6,
  5,
  10,
  9,
};

// Matrix for solving a given lights out game
// This is a matrix of 1's and 0's packed into 16 bit integers

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

// prog_uint16_t uint16_t matrix_a[16][16] = {
//   {1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0},
//   {1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0},
//   {0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0},
//   {1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1},
//   {1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
//   {0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0},
//   {0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0},
//   {0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0},
//   {0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0},
//   {0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0},
//   {0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0},
//   {0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1},
//   {1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1},
//   {0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0},
//   {0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1},
//   {0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,1} };


/* Mini Lights Out Levels */

prog_uint16_t mini_lightsout_levels[] = {
  0xA0A0,
  0xA0A0,
  0xF99F,
  0x4422,
  0xEEEE,
  0xDCAC,
  0xF741,
  0xF7DE,
  0xF7EB,
  0xAAEE,
  0xDCCD,
  0xB5AD,
  0xCCEE,
  0x256C,
  0x6236,
  0xE63,
  0x91B1,
  0x9669,
  0x6996,
  0xFF00,
  0xD436,
  0x7D89,
  0x7D0D,
  0xA141,
  0x4BD9,
  0x884B,
  0xB424,
  0x6886,
  0x4A52,
  0x4820,
  0xA5A4,
  0x7FFF,
  0x4801
};

/* 1010000010100000 */
/* 1111100110011111 */
/* 0100010000100010 */
/* 1110111011101110 */
/* 1101110010101100 */
/* 1111011101000001 */
/* 1111011111011110 */
/* 1111011111101011 */
/* 1010101011101110 */
/* 1101110011001101 */
/* 1011010110101101 */
/* 1100110011101110 */
/* 0010010101101100 */
/* 0110001000110110 */
/* 0000111001100011 */
/* 1001000110110001 */
/* 1001011001101001 */
/* 0110100110010110 */
/* 1111111100000000 */
/* 1101010000110110 */
/* 0111110110001001 */
/* 0111110100001101 */
/* 1010000101000001 */
/* 0100101111011001 */
/* 1000100001001011 */
/* 1011010000100100 */
/* 0110100010000110 */
/* 0100101001010010 */
/* 0100100000100000 */
/* 1010010110100100 */
/* 0111111111111111 */
/* 0100100000000001 */

prog_uint16_t animation1[] = {
0x0000,
0x8000,
0xC800,
0xEC80,
0xFEC8,
0xFFEC,
0xFFFE,
};

/*
obase=16;ibase=2;
0000
0000
0000
0000

1000
0000
0000
0000

1100
1000
0000
0000

1110
1100
1000
0000

1111
1110
1100
1000

1111
1111
1110
1100

1111
1111
1111
1110
*/

prog_uint16_t animation2[] = {
0x0000,
0x1000,
0x3100,
0x7310,
0xF731,
0xFF73,
0xFFF7
};

/*
obase=16;ibase=2;
0000
0000
0000
0000

0001
0000
0000
0000

0011
0001
0000
0000

0111
0011
0001
0000

1111
0111
0011
0001

1111
1111
0111
0011

1111
1111
1111
0111

*/

// vim: ft=cpp
