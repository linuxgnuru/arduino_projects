const char alphabetArray[] = {
  0b11010111, // A
  0b00011111, // B
  0b10001101, // C
  0b01011110, // D
  0b10001111, // E
  0b10000111, // F
  0b10011101, // G
  0b01010111, // H
  0b01010000, // I
  0b01011100, // J
  0b01110111, // K (H.)
  0b00001101, // L
  0b00110110, // M (n.) c b e f
  0b00010110, // N (n)  c b e
  0b11011101, // O
  0b11000111, // P
  0b11111101, // Q (O.)
  0b00000110, // R (r)
  0b10011011, // S
  0b11010000, // T
  0b01011101, // U
  0b00011100, // V
  0b00111100, // W
  0b01010101, // X
  0b01011011, // Y
  0b11001110, // Z (2)
  0b00000000 // space
};

const char digitArray[] = {
  0b11011101, // 0
  0b01010000, // 1
  0b11001110, // 2
  0b11011010, // 3
  0b01010011, // 4
  0b10011011, // 5
  0b10011111, // 6
  0b11010000, // 7
  0b11011111, // 8
  0b11011011, // 9
  0b00100000  // .
};

const char digitArray4[] = {
  0b01111011, // 0
  0b00001010, // 1
  0b11110010, // 2
  0b10111010, // 3
  0b10001011, // 4
  0b10111001, // 5
  0b11111001, // 6
  0b00011010, // 7
  0b11111011, // 8
  0b10111011  // 9
};

typedef enum {
  S_DIGIT,
  S_RESULT,
  S_BOTH
} clear_enum;

typedef enum {
  LED_G,
  LED_R,
  LED_OFF
} status_enum;

typedef enum {
  LED_0,
  LED_1,
  LED_2,
  LED_3,
  LED_4,
  LED_5,
  LED_6
} led_enum;

const int alertPin = 13;

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

const int dataPinA  = 8; // Pin connected to DS of 74HC595 - blue
const int latchPinA = 9; // Pin connected to ST_CP of 74HC595 - green
const int clockPinA = 10; // Pin connected to SH_CP of 74HC595 - yellow

const int buttonPinA = 5;
const int buttonPinB = 6;
const int buttonPinC = 7;

