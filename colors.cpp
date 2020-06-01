#define hex2red(clr)   ((clr >> 16) & 0xFF)
#define hex2green(clr) ((clr >>  8) & 0xFF)
#define hex2blue(clr)  ((clr >>  0) & 0xFF)

const int HEX_BLACK = 0x000000;
const int HEX_WHITE = 0xFFFFFF;
const int HEX_RED   = 0xFF0000;
const int HEX_GREED = 0x00FF00;
const int HEX_BLUE  = 0x0000FF;
const int HEX_YELLOW= 0xFFFF00;
const uint8_t CODE_BLACK = 0;
const uint8_t CODE_WHITE = 1;
const uint8_t CODE_RED   = 2;
const uint8_t CODE_GREEN = 3;
const uint8_t CODE_YELLOW= 5;

const int colors[6] = {
  HEX_BLACK, // black
  HEX_WHITE, // white
  HEX_RED, // red
  HEX_GREED, // greed
  HEX_BLUE, // blue
  HEX_YELLOW
};