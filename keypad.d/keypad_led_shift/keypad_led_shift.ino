/*

  7 segment LED + shift register (74HC595) + keypad
  
  7-segment LED Pins:
  --    A
 |  | B   G
  --    C
 |  | D   F
  -- .  E   H (.)

 B  1   8  A
 C  2   7  G
 X  X   X
GND G   X
 X  X   X
 D  3   6  H (.)
 E  4   5  F

   shift register Pins:
	      +--------+
	1out -+  1  16 +- Vin
	2out -+  2  15 +- 0out
	3out -+  3  14 +- data (blue)
	4out -+  4  13 +- ground
	5out -+  5  12 +- latch (green + 1u cap if first)
	6out -+  6  11 +- lock (yellow)
	7out -+  7  10 +- Vin
	 gnd -+  8   9 +- serial out
	      +--------+
0out = 0b10000000
1out = 0b01000000
2out = 0b00100000
...
6out = 0b00000010
7out = 0b00000001

   AG.FEDCB
 0b00000000
                  AG FED B
 0 = ABDEFG   = 0b11011101
                   G F
 1 = FG       = 0b01010000
                  AG  EDC
 2 = ACDEG    = 0b11001110
                  AG FE C
 3 = ACEFG    = 0b11011010
                   G F  CB
 4 = BCFG     = 0b01010011
                  A  FE CB
 5 = ABCEF    = 0b10011011
                  A  FEDCB
 6 = ABCDEF   = 0b10011111
                  AG F
 7 = AFG      = 0b11010000
                  AG FEDCB
 8 = ABCDEFG  = 0b11011111
                  AG FE CB
 9 = ABCEFG   = 0b11011011
                  AG.F DCB
 A = ABCDFGH  = 0b11010111
                  AG.FEDCB
 B = ABCDEFGH = 0b11111111
                  A . ED B
 C = ABDEH    = 0b10101101
                  AG.FED B
 D = ABDEFGH  = 0b11111101
                  AG.FEDCB
                0b01110111
 * = ABDEFGH  = 0b11110101
                  AG.FED B
                0b10110110
 # = ABDEFGH  = 0b10110110
 */
#include <Keypad.h>

//Pin connected to SH_CP of 74HC595
int clockPin = A1; // yellow
//Pin connected to ST_CP of 74HC595
int latchPin = A2; // green
//Pin connected to DS of 74HC595
int dataPin = A3; // blue

byte digitArray[] = {
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
  0b11010111, // A
  0b11111111, // B
  0b10101101, // C
  0b11111101, // D
  0b01110111, // *
  0b10110110  // #
};

const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = { 2, 3, 4, 5 }; //connect to row pinouts 
byte colPins[COLS] = { 6, 7, 8, 9 }; //connect to column pinouts

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

byte data;

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  blinkAll_2Bytes(4, 200);
}

void loop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    switch (key)
    {
      case '0': data = digitArray[0]; break;
      case '1': data = digitArray[1]; break;
      case '2': data = digitArray[2]; break;
      case '3': data = digitArray[3]; break;
      case '4': data = digitArray[4]; break;
      case '5': data = digitArray[5]; break;
      case '6': data = digitArray[6]; break;
      case '7': data = digitArray[7]; break;
      case '8': data = digitArray[8]; break;
      case '9': data = digitArray[9]; break;
      case 'A': data = digitArray[10]; break;
      case 'B': data = digitArray[11]; break;
      case 'C': data = digitArray[12]; break;
      case 'D': data = digitArray[13]; break;
      case '*': data = digitArray[14]; break;
      case '#': data = digitArray[15]; break;
    }
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data);
    digitalWrite(latchPin, 1);
  }
  delay(100);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int i = 0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

void blinkAll_2Bytes(int n, int d)
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(50);
  for (int x = 0; x < n; x++)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

