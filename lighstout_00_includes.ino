#include <avr/pgmspace.h>

#include <SPI.h>
#include "max6954.h"

#include "Adafruit_WS2801.h"

#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define STRIP_CLK  5
#define STRIP_DATA 6
#define CS1        9  // MAX6954 1
#define CS2        8  // MAX6954 2
#define DATAOUT    11 // MOSI
#define DATAIN     12 // MISO
#define CLK        13 // sck

