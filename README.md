## Lights Out Arcade Box

Over the past year I've been working on my first big hardware project. It
started as an experiment in powering and controlling led displays. I love the
blinky lights and wanted something big that would catch the eye.

[![Lights Out](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3852.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3852.JPG)

The end result is a tabletop sized arcade for a 4x4 lights out game similar in
gameplay to the
[Mini Lights out Game](http://www.jaapsch.net/puzzles/lights.htm#descmini)
by Tiger Electronics.

Checkout a video demo of the gameplay [here on vimeo](https://vimeo.com/59807162).

This project took me a long while to complete due to lack of free time. I began
when my son turned one year old and finished not long before he turned two. The
effort paid off though and I'm happy with how it turned out.

### Parts

I took the lego approach while putting this together. Pick powerful components
and wire them up. Here is a list of every part used and what I paid for it.

| Price    | Quantity | Item                                                                                                                                                        |
| -------- | -------- | -----------------------------------------------------------------------------------------------------------------------------------------------------       |
| $02.50   |          | [5/64 inch Balldriver](http://shop.evilmadscientist.com/productsmenu/partsmenu/202)                                                                         |
| $02.85   |          | [ATMEGA328P](https://www.adafruit.com/products/123)                                                                                                         |
| $02.95   |          | [MCP23017 - i2c 16 input/output port expander](https://www.adafruit.com/products/732)                                                                       |
| $04.50   | 2        | [4xAA Battery Holder](http://shop.evilmadscientist.com/productsmenu/partsmenu/422)                                                                          |
| $05.00   |          | Various resistors and capacitors mostly 0.1µF                                                                                                              |
| $05.95   |          | [Waterproof Metal On/Off Switch with Green LED Ring](http://adafruit.com/products/482)                                                                      |
| $06.46   |          | [6-32 x 3/8 inch Button Socket Cap Head Screw](http://shop.evilmadscientist.com/productsmenu/partsmenu/468-632bscs)                                         |
| $06.95   |          | [Premium Female/Female Jumper Wires - 40 x 6"](https://www.adafruit.com/products/266)                                                                       |
| $06.99   |          | [Wire-Wrapping Tool](http://www.radioshack.com/product/index.jsp?productId=2103243)                                                                         |
| $10.20   |          | [Right Angle Mounting Bracket](http://shop.evilmadscientist.com/productsmenu/partsmenu/465-bracket)                                                         |
| $14.95   |          | [FTDI Friend + extras - v1.0](https://www.adafruit.com/products/284)                                                                                        |
| $14.95   |          | [Pololu Step-Down Voltage Regulator D15V35F5S3](http://www.pololu.com/catalog/product/2110)                                                                 |
| $21.90   |          | [10x30ft of kynar wire](http://www.ebay.com/itm/10-X-30-ft-Kynar-30-awg-wire-10-Color-4-xbox-v-game-/260623604297)                                          |
| $26.40   | 16       | [0.8 inch Alphanumeric Displays](http://shop.evilmadscientist.com/productsmenu/partsmenu/232)                                                               |
| $32.96   |          | [Clear Lasercut Acrylic from Ponoko](http://www.ponoko.com/make-and-sell/show-material/72-acrylic-clear)                                                    |
| $34.18   |          | [Wire wrap sockets](http://www.king-cart.com/phoenixent/product=SOCKETS+WIRE+WRAP+DIP+%2526+SIP/exact_match=exact) Part Numbers: HWS3089, HWS15765, HWS1462 |
| $39.95   |          | [12mm Diffused Thin Digital RGB LED Pixels (Strand of 25) - WS2801](https://www.adafruit.com/products/322)                                                  |
| $42.56   | 16       | [Arcade Button - 30mm Translucent Clear](https://www.adafruit.com/products/471)                                                                             |
| [Free Sample](https://shop.maximintegrated.com/storefront/searchsample.do?menuitem=Sample&event=SampleSearchLoad) | 2  | [MAX6954 4-Wire Interfaced, 2.7V to 5.5V LED Display Driver with I/O Expander and Key Scan](http://www.maximintegrated.com/datasheet/index.mvp/id/3410)  |

Total: $282.20

Most of these parts are overkill and I'm sure this project could be accomplished
cheaper and more efficiently. Some parts don't seem to be available anymore.
Even so, it has a lot of character and thats what I was going for.

### Board Layout

Here you can see the layout of the components. I couldn't find a through hole
protoboard that was large enough to accommodate all of the alphanumeric digits so
I fastened two together and used some legos attached to with screws for
reinforcement.

[![Board Layout](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3878.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3878.JPG)

### The 17-segment displays

[![Display Closeup](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3863.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3863.JPG)

![MAX6954 Connection Circuit](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/MAX6954_Connection_Circuit.png)

Below the alphanumeric digits are the MAX6954 drivers. There is one driver per
row. Next to each driver are decoupling capacitors and a resistor for limiting
the current to each segment. The diagram to the right details the required
connections. These are pretty beefy chips. They do a lot and have a built in
font map that correspond to ASCII character codes. Getting characters to display
is as simple as sending a string over the SPI connection.

The MAX6954 datasheet claims they are chainable but I was getting garbage on the
second display when wiring the SPI bus in a chain. It worked much better when
each chip had it's own chip select line and I sent commands to both or one at a
time.

For extra fun I also wanted to see some lowercase characters so I created my own
font map and displayed it by turning individual segments on and off.

![Lowercase Font Animation](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/lowercase.gif)

Connecting each chip to 8 digits was the hardest part of this build.  Wire
wrapping that many connections took about 6hrs total. Below is the connection
scheme in a table from the datasheet. The chip uses charlie-plexing to control
that many segments. Most of the MAX6954 pins are connected to a single segment
on each digit. Pins 0-7 are connected to two segments.

[![MAX6954 Display Circuit](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/MAX6954_Connections_for_8_16-Segment_Digits.png)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/zD-R6pwl56#f:MAX6954%20Connections%20for%208%2016-Segment%20Digits.png)

Before I began I drew the connections with pencil and paper. I picked different
colored wire for each row and tested that each point was connected properly
using a multimeter. Thankfully I didn't have to repair any connections after
finishing.

[![Wire wrapped rows of digits](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3869.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3869.JPG)

[![Wire wrapped rows of digits close up](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3875.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3875.JPG)

I chose to wire wrap the whole thing because I wasn't 100% sure how to connect
everything to start with. Unfortunately I'm lacking a proper circuit schematic.
Learning how to use Eagle or gEDA is still on my todo list. I was able to get
away with this because the connections outside of the digits are relatively
simple and I could rewire anything quickly if needed.

### Buttons and RGB LEDs

The remaining components include a MCP23017 port expander, 16 buttons and a RGB
LED strand with 16 pixels for the button grid. I was a bit lazy and decided to
use the MCP23017 to read the state of each individual button rather than create
a button array wired to one of the analog inputs of the ATMEGA328P. Each button
is connected to one pin of the MCP23017 and ground. In the code it's very simple
to get the state of each button as a 16 bit unsigned integer.

[![Rainbow colors on the RGB LEDs](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3860.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3860.JPG)

[![Button panel removed](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3881.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3881.JPG)

The RGB LED strip is dead simple to use with the [tutorial and provided library](http://learn.adafruit.com/12mm-led-pixels/).
The strip isn't arranged in the same order as I was representing the game board
in the code. To deal with that I store the position of each light on the
strip in the correct location in an array. I then use bit masking and shifting to determine
if a single bit in the 16-bit unsigned integer is a 1 and if so set the light to
on. Here is the loop I use to turn the lights on and off for a given board:

Matrix for solving a given lights out game [Source File](https://github.com/AnthonyDiGirolamo/lightsout/blob/master/lightsout_02_helper_functions.ino)

```c
void print_board(uint16_t board, uint32_t lit_color) {
  uint16_t lit = 0;
  for(int i=15; i>=0; i--) {
    lit = (board & space_masks[i]) >> i;
    strip.setPixelColor(board_light_index[i], (lit ? lit_color : lights_out_color_schemes[current_scheme+1]));
  }
  strip.show();
}
```

### Color Chooser

In addition to the lights out game I wrote up a quick color chooser. With it you
can increase or decrease the amount of red green and blue and see the hex color
code. There is also a button to generate a random color and another to start a
rainbow color rotation if you are in need of a dopamine squirt. Once you have a
color you like you can save it in the EEPROM and have it used in your next
lights out game. Checkout [the demo](https://vimeo.com/59807162) for an example of how it works.

[![Rainbow colors on the RGB LEDs](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3861.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3861.JPG)

### Solving a Game

One feature I really wanted was to be able to display the solution to a given
board. A [post on Merlin's magic square](http://www.cut-the-knot.org/Curriculum/Games/Merlin4x4.shtml#theory) at
cut-the-knot.org pointed me to an article in Mathematics Magazine, Vol. 74
[An Easy Solution to Mini Lights Out(pp. 57-59) Jennie Missigman and Richard Weida](http://www.jstor.org/stable/2691157)
The gist of this article points out a few unique properties of 4x4 lights out
games that are periodic (wrap around).

1. Every game is solvable
2. The order of buttons pressed does not matter
3. The solution to any game can be found by multiplying a matrix by a column
   vector that represents the status of the board. 1's represent lights turned
   on, 0's off.

Armed with this knowledge I was able to add random levels (which are all
solvable), a solution function, and the total number of moves required to win.

At first I planned to generate all levels randomly but it turns out that random
levels are extremely hard to win. They have no patterns that aid in solving. To
address that I wanted to include the original levels that were part of the
[Mini Lights out Game](http://www.jaapsch.net/puzzles/lights.htm#descmini).
You can find them at
[Mini Lights Out Levels and Solutions](http://www.jaapsch.net/puzzles/extra/lomini.txt)
on
[Jaap's Puzzle Page](http://www.jaapsch.net/puzzles/lightsol.htm#lomini).

### Code

The [Lights Out Arduino Sketch](https://github.com/AnthonyDiGirolamo/lightsout)
is mostly well organized. I've also written a
[library for controlling the MAX6954's](https://github.com/AnthonyDiGirolamo/MAX6954).
which keeps the lights out code simpler.

The sketch is organized into separate files. When compiled in the Arduino IDE
the files are concatenated and then compiled as a single source file. The
lights out and color chooser functions are broken out into their own classes.
All strings and levels are stored in program memory.

Each level, and the state of the board, is represented by a 16-bit unsigned
integer. Literally 16 1's or 0's. This makes it take up very little space. To
compute a solution for a given board a matrix vector multiplication needs to be
performed. The 16x16 matrix is composed of only 1's and 0's so to save space I
store it as an array of 16 bit integers.

Matrix for solving a given lights out game [Source File](https://github.com/AnthonyDiGirolamo/lightsout/blob/master/lightsout_01_vars.ino)

```c
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

// The same matrix but stored as a 16x16 array
// prog_uint16_t uint16_t matrix_a[16][16] = {
// {1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0},
// {1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0},
// {0,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0},
// {1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1},
// {1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
// {0,1,0,0,1,1,1,0,0,1,0,0,0,0,0,0},
// {0,0,1,0,0,1,1,1,0,0,1,0,0,0,0,0},
// {0,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0},
// {0,0,0,0,1,0,0,0,1,1,0,1,1,0,0,0},
// {0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0},
// {0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0},
// {0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1},
// {1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,1},
// {0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0},
// {0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1},
// {0,0,0,1,0,0,0,0,0,0,0,1,1,0,1,1} }
```

Packing the matrix value like this adds a bit of code to the matrix vector
multiply. It's still quite readable though. If you are unfamiliar with bitmath
checkout [The Arduino Playground Bit Math Tutorial](http://playground.arduino.cc/Code/BitMath)
and [Bit Twiddling Hacks](http://graphics.stanford.edu/~seander/bithacks.html)

Matrix Vector Multiply [Source File](https://github.com/AnthonyDiGirolamo/lightsout/blob/master/lightsout_04_lights_out_class.ino)

```c
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
    solution |= (x << (15-r)); // Pack
  }
  return solution;
}
```

As a side note, it's pretty easy to convert number bases in your editor if you
can filter text through an external program. This is trivial in vim and is
possible with other editors like TextMate. In vim, I filter the following text
through `bc` "An arbitrary precision calculator language" available on almost
any Linux distribution and MacOSX. The first two lines tell bc to output base 16
and take base 2 as input (they should be set in that order).

Base 2 input
```
obase=16;ibase=2
1101100000001000
1110010000000100
0111001000000010
1011000100000001
1000110110000000
0100111001000000
0010011100100000
0001101100010000
0000100011011000
0000010011100100
0000001001110010
0000000110110001
1000000010001101
0100000001001110
0010000000100111
0001000000011011
```

The result, instant base 16 without leaving the editor!

Base 16 output
```
D808
E404
7202
B101
8D80
4E40
2720
1B10
8D8
4E4
272
1B1
808D
404E
2027
101B
```

### Enclosure

This is the first time I've tried to design any kind of enclosure. I started by
picking a material on Ponoko for laser-cutting. Once I knew what I was working
with I began creating panels in [Sketchup](http://www.sketchup.com/) and tried
to make a box that would fit the circuit board and accommodate 16 buttons.

[![Enclosue Rendering](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/16segment_board_enclosure.png)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:16segment_board_enclosure.png)

[![Enclosue Rendering 2](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/16segment_board_enclosure2.png)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:16segment_board_enclosure2.png)

I was aware of the
[tab in slot method](http://support.ponoko.com/entries/20344437-laser-cut-project-box-tutorial)
for designing an enclosure but by this time I had a lot of project fatigue and
decided to put put it all together with angle brackets. It was easy to create
the angle brackets in sketchup so I could have the screw holes precut.  One
thing I should have done is round the corners of each panel. The right angles
are sharp! I ended up sanding each corner until they were rounded by hand.

After finishing the design I flattened each panel and arranged it to fit in the
Ponoko P3 sized order using the [SVG Sketchup plugin](http://code.google.com/p/sketchup-svg-outline-plugin/downloads/list).
You can [download the sketchup file](/downloads/16segment_board_enclosure.zip)
and have a look if you wish.

### Improvements

[![Display Close Up](https://raw.githubusercontent.com/AnthonyDiGirolamo/lightsout/master/images/IMG_3864.JPG)](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#f:IMG_3864.JPG)

There are definitely some improvements I can still make. If I were to build this
whole thing again from scratch I would make these changes:

* Design a circuit board instead of wire wrapping
* Tab-in-slot method enclosure
* Better arcade buttons with a more clicky feel
* Better way to fasten the RGB LEDs to the buttons, maybe add additional holes
in the enclosure.
* A red contrast filter for the 17 segment characters, they are way too bright.
* Better button debouncing code

Thanks for reading! You can find [more pictures at full resolution](https://www.dropbox.com/sh/jtv7i2y51uckmn1/6SccJowgE1/Full%20Size#/)
in my dropbox.
