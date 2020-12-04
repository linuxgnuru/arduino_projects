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

//#define DEBUG 1

const byte LATCH = 10;

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
  SPI.begin();
  pinMode(LATCH, OUTPUT);
}

int countdown = 1000;

void loop()
{
  printDigit(0, countdown / 1000, false);
  printDigit(1, (countdown % 1000) / 100, true);
  printDigit(2, (countdown % 100) / 10, false);
  printDigit(3, (countdown % 100) % 10, false);
  countdown--;
  if (countdown < 0)
  {
    for (int j = 0; j < 10; j++)
    {
    for (int i = 0; i < 4; i++)
      clearD(i);
      delay(100);
    for (int i = 0; i < 4; i++)
      printDigit(i, 0, true);
      delay(100);
    }
    countdown = 1000;
  }
  delay(15);
  /*
    int h, m;
    boolean pm = false;
    //DateTime now = RTC.getTime();

    h = RTC.getHour();
    if (h >= 12) pm = true;
    if (h > 12) h -= 12;
    m = RTC.getMinute();
    printDigit(0, h / 10, false);
    printDigit(1, h % 10, true);
    printDigit(2, m / 10, false);
    printDigit(3, m % 10, pm);
  */
}

// pos = 0 - 3
void printDigit(int pos, int num, boolean dp)
{
  int ctr = 0;
  byte myData;

  if (pos < 4 && (num > -1 && num < 10 || num == 99))
  {
    //Serial.println(num);
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

