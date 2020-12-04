typedef enum {
  RPIN,
  GPIN,
  BPIN
} rgb_enum;

typedef enum {
  POT,
  LDR,
  TMP,
  TLT,
  BUZ,
  RGB,
  DIG,
  BAR
} IO_enum;

typedef struct {
  long fu;
  int bar;
  char c;
} testStruct;

char digitArray[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01010111, // 3
  0b01100110, // 4
  0b01110101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01110111  // 9
};

