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
char string_win[]      PROGMEM = "You Win ";
char string_perfect[]  PROGMEM = "Perfect ";
char string_level[]    PROGMEM = "Level   ";
char string_board[]    PROGMEM = "Board   ";
char string_solution[] PROGMEM = "Solution";
char string_moves[]    PROGMEM = "Moves   ";
char string_testing[]  PROGMEM = "TEST.TEST";

// Main Menu
char string_mainmenu[] PROGMEM = "Main.Menu";
char string_lights[]   PROGMEM = "Lights  ";
char string_out[]      PROGMEM = "Out     ";
char string_color[]    PROGMEM = "Color   ";
char string_memory[]   PROGMEM = "Memory  ";
char string_picker[]   PROGMEM = "Chooser ";
char string_fade[]     PROGMEM = "Rainbow ";
char string_full[]     PROGMEM = "Full    ";
char string_half[]     PROGMEM = "Half    ";

#define MENU_DELAY 2000
#define MAINMENU 0
#define LIGHTS 1
#define PICKER 2
#define MEMORY 3

uint8_t mode = MAINMENU;

// Color Schemes
prog_uint32_t main_menu_color_schemes[] = {
  0x040404, // off
  0xFF9000, // item 4
  0x0000FF, // item 3
  0xFF0000, // item 1
  0x00FF00, // item 2
};

prog_uint32_t lights_out_color_schemes[] = {
  0xFF9000, // on
  0x010101, // off
  0xFFFFFF, // on
  0x010101, // off
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

// vim: ft=cpp
