#include <Wire.h>
#include <RTClib.h>
#include <OneWire.h>

RTC_PCF8563 RTC;

#define DS_PIN 10

OneWire ds(DS_PIN);

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow wire

const byte digitArray[] = {
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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  Wire.begin();
  RTC.begin();
  // following line sets the RTC to the date & time this sketch was compiled
  //  RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
  int h, m;
  boolean pm = false;

  DateTime now = RTC.now();
  h = now.hour();
  if (h >= 12)
    pm = true;
  if (h > 12)
    h -= 12;
  m = now.minute();
  printDigit(0, h / 10, false);
  printDigit(1, h % 10, true);
  printDigit(2, m / 10, false);
  printDigit(3, m % 10, (pm == true ? true : false));
  delay(100);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
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
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    digitalWrite(latchPin, 1);
  }
}



