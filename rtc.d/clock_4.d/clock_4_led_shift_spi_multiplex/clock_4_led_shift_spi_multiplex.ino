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

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>

RTC_PCF8563 RTC;

#define DIG_NUM 4

//#define DEBUG 1

//const int mdyPin = 3;

const byte LATCH = 10;

const int rtcVccPin = A2;
const int rtcGndPin = A3;

int del = 55; // delay value

int cols[DIG_NUM];

const byte ledNum[] = {
  //abcdefg
  0b11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110  // 9
};

byte data = 0b00000000;

boolean pm = false;

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  for (int i = 0; i < DIG_NUM; i++)
  {
    pinMode(i + 3, OUTPUT);
    cols[i] = i + 3;
  }
  pinMode(rtcGndPin, OUTPUT);
  pinMode(rtcVccPin, OUTPUT);
  digitalWrite(rtcGndPin, LOW);
  digitalWrite(rtcVccPin, HIGH);
  Wire.begin();
  RTC.begin();
  SPI.begin();
  // following line sets the RTC to the date & time this sketch was compiled
//  RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
  int h, m;

  DateTime now = RTC.now();
  h = now.hour();
  if (h >= 12) pm = true;
  if (h > 12) h -= 12;
  m = now.minute();
  printDigit(0, h / 10, false);
  delayMicroseconds(del);
  printDigit(1, h % 10, true);
  delayMicroseconds(del);
  printDigit(2, m / 10, false);
  delayMicroseconds(del);
  printDigit(3, m % 10, (pm == true ? true : false));
  delayMicroseconds(del);
}

// pos = 0 - 3
void printDigit(int pos, int num, boolean dp)
{
  if (pos < 4 && (num > -1 && num < 10 || num == 99))
  {
    data = (num == 99 ? 0b00000000 : ledNum[num]);
  // next, clear everything
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00000000);
  digitalWrite(LATCH, HIGH);
  for (int i = 0; i < DIG_NUM; i++)
    digitalWrite(cols[i], HIGH);
  digitalWrite(cols[pos], LOW);
    if (dp == true)
      bitSet(data, 0);
    digitalWrite(LATCH, LOW);
    SPI.transfer(data);
    digitalWrite(LATCH, HIGH);
  }
}

