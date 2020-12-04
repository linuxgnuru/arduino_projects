#include <SPI.h>
#include "segment_digit.h"

//#define DEBUG 1

const byte LATCH = 10;

// for shift register leds:
byte data[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

void setup()
{
  pinMode(LATCH, OUTPUT);
  SPI.begin();
}

void loop()
{
  for (int j = 0; j < 10; j++)
  {
    printDigit(0, j, true);
    printDigit(1, j, true);
    printDigit(2, j, true);
    printDigit(3, j, true);
    delay(250);
  }
  /*
    int tens, ones, tmp, decpA, decpB;
    float mypi = 77.77;
    tens = (int)mypi / 10;
    ones = (int)mypi % 10;
    tmp = mypi * 10;
    decpA = (int)tmp % 10;
    decpB = (int)tmp % 100;
    if (decpB > 10)
      decpB /= 10;
    printDigit(0, tens, false);
    printDigit(1, ones, true);
    printDigit(2, decpA, false);
    printDigit(3, decpB, false);
    delay(1000);
  */
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
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  digitalWrite(LATCH, HIGH);
}

