#include <SPI.h>

//#define DEBUG 1

const byte LATCH = 10;

const byte alphabetArray[] = {
  0b11011011, // A
  0b11101001, // B
  0b01110001, // C
  0b11101011, // D
  0b11110001, // E
  0b11010001, // F
  0b01111001, // G
  0b11001011, // H
  0b01000001, // I
  0b01101000, // J
  0b11001111, // K (H.)
  0b01100001, // L
  0b01011111, // M (n.)
  0b01011011, // N (n)
  0b11101000, // O
  0b11010011, // P
  0b01111111, // Q (O.)
  0b11000000, // R (r)
  0b10111001, // S (5)
  0b10001000, // T
  0b01101011, // U
  0b01101000, // V
  0b01101100, // W
  0b01001011, // X
  0b10101011, // Y
  0b11110010, // Z (2)
  0b00000000 // space
};

const byte digitArray[] = {
//  BCDGE.FA
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

// for shift register leds:
byte data[] = { 
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  SPI.begin();
}

void loop()
{
  int h, m;
  boolean pm;
  h = 18;
  m = 35;
  printDigit(0, h / 10, false);
  printDigit(1, h % 10, true);
  printDigit(2, m / 10, false);
  printDigit(3, m % 10, (pm == true ? true : false));
}

// pos = 0 - 3
void printDigit(int pos, int num, boolean dp)
{
  int ctr = 0;
  byte myData;

  if (pos < 4 && (num > -1 && num < 10 || num == 99))
  {
    myData = (num == 99 ? 0b00000000 : digitArray[num]);
    if (dp == true)
      bitSet(myData, 2);
    data[pos] = myData;
    digitalWrite(LATCH, LOW);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    digitalWrite(LATCH, HIGH);
  }
}

void clearD(int pos)
{
  int ctr = 0;

  data[pos] = 0b00000000;
  digitalWrite(LATCH, LOW);
  SPI.transfer(data[ctr++]);
  SPI.transfer(data[ctr++]);
  SPI.transfer(data[ctr++]);
  SPI.transfer(data[ctr++]);
  digitalWrite(LATCH, HIGH);
}

