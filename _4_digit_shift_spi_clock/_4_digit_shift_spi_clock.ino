/*
  4 digit (via shift registers) temperature

  sample code:

  shiftOut(dataPin, clockPin, digitArray[k - 48]); // char k = '5';
  shiftOut(dataPin, clockPin, alphabetArray[c - 65]);  // char c = 'A' ... 'Z'
  ---     A
  |   |  F   B
  ---     G
  |   |  E   C
  ---     D    dp

*/

#include <SPI.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h>

//#define DEBUG 1
#define AM 0
#define PM 1

Rtc_Pcf8563 RTC;

const byte LATCH = 10;
boolean am_pm = AM;

unsigned long lastMillis = 0;

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

boolean flipFlop = true;

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  Wire.begin();
}

void loop()
{
  int h, m;
  boolean pm = false;
  //DateTime now = RTC.getTime();
  h = RTC.getHour();
  m = RTC.getMinute();
  if (h >= 12)
  {
    pm = true;
    if (h > 12)
      h -= 12;
  }
  m = RTC.getMinute();
  printDigit(0, h / 10, false);
  printDigit(1, h % 10, false);
  printDigit(2, m / 10, false);
  printDigit(3, m % 10, pm);
  blinkDp();
}

void blinkDp()
{
  unsigned long currentMillis = millis();
  byte mData = data[1];

  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    if (flipFlop)
    {
      bitSet(mData, 2);
      flipFlop = false;
    }
    else
    {
      bitClear(mData, 2);
      flipFlop = true;
    }
    data[1] = mData;
    digitalWrite(LATCH, LOW);
    for (int i = 0; i < 4; i++)
      SPI.transfer(data[i]);
    digitalWrite(LATCH, HIGH);
  }
}

// pos = 0 - 3
void printDigit(int pos, int num, boolean dp)
{
  byte myData;

  if (pos < 4 && (num > -1 && num < 10 || num == 99))
  {
    //Serial.println(num);
    myData = (num == 99 ? 0b00000000 : digitArray[num]);
    if (dp == true)
      bitSet(myData, 2);
    data[pos] = myData;
    digitalWrite(LATCH, LOW);
    for (int i = 0; i < 4; i++)
      SPI.transfer(data[i]);
    digitalWrite(LATCH, HIGH);
  }
}

void clearD(int pos)
{
  data[pos] = 0b00000000;
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < 4; i++)
    SPI.transfer(data[i]);
  digitalWrite(LATCH, HIGH);
}

