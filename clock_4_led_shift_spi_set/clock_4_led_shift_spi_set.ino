#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <SPI.h>

#include <ctype.h>
#include <string.h>

#include "vars.h"

Rtc_Pcf8563 RTC;

//#define DEBUG
//#define SET_CLOCK

#define TIME_ 0
#define DATE_ 1

const byte datePin = 3;
const byte twentyPin = 4;
const byte setPin = 5;

const byte LATCH = 10;

int buttonStateDate;             // the current reading from the input pin
int lastButtonStateDate = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTimeDate = 0;  // the last time the output pin was toggled

int buttonStateTwenty;             // the current reading from the input pin
int lastButtonStateTwenty = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTimeTwenty = 0;  // the last time the output pin was toggled

unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// for shift register leds:
byte data[4];

boolean date_time = TIME_;
boolean twenty_four = false;
boolean year_show = true;

void setup()
{
  for (int i = 0; i < 4; i++)
    data[i] = 0b00000000;
  pinMode(datePin, INPUT_PULLUP);
  pinMode(twentyPin, INPUT_PULLUP);
  pinMode(setPin, INPUT_PULLUP);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  Wire.begin();
  //RTC.begin();
  SPI.begin();
#ifdef SET_CLOCK
  RTC.initClock();
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  RTC.setDate(25, 5, 5, 0, 17);
  //hr, min, sec
  RTC.setTime(12, 03, 00);
#endif
}

#ifdef DEBUG
unsigned long lm = 0;
#endif

void loop()
{
#ifdef DEBUG
  unsigned long cm = millis();
  #endif
  int readingDate = digitalRead(datePin);
  int readingTwenty = digitalRead(twentyPin);

#ifdef DEBUG
  if (cm - lm >= 1000)
  {
    lm = cm;
    Serial.println(RTC.getYear());
  }
  #endif
  if (readingDate != lastButtonStateDate)
    lastDebounceTimeDate = millis();
  if ((millis() - lastDebounceTimeDate) > debounceDelay)
  {
    if (readingDate != buttonStateDate)
    {
      buttonStateDate = readingDate;
      if (buttonStateDate == LOW)
        date_time = !date_time;
    }
  }
  lastButtonStateDate = readingDate;
  if (readingTwenty != lastButtonStateTwenty)
    lastDebounceTimeTwenty = millis();
  if ((millis() - lastDebounceTimeTwenty) > debounceDelay)
  {
    if (readingTwenty != buttonStateTwenty)
    {
      buttonStateTwenty = readingTwenty;
      if (buttonStateTwenty == LOW)
        twenty_four = !twenty_four;
    }
  }
  lastButtonStateTwenty = readingTwenty;
  printDateTime();
  
}

void printDateTime()
{
  int a, b;

  if (date_time == DATE_)
  {
    a = RTC.getDay();
    b = RTC.getMonth();
    printDigit(0, a / 10, false);
    printDigit(1, a % 10, true);
    printDigit(2, b / 10, false);
    printDigit(3, b % 10, false);
    if (year_show)
    {
      delay(2000);
      year_show = false;
      a = RTC.getYear();
      b = 2;
      printDigit(0, b, false);
      b = (a / 100) - ((a / 1000) * 10);
      printDigit(1, b, false);
      b = (a / 10) - ((a / 1000) * 100) - (((a / 100) - ((a / 1000) * 10)) * 10);
      printDigit(2, b, false);
      b = a % 10;
      printDigit(3, b, false);
      delay(2000);
    }
  }
  else
  {
    year_show = true;
    a = RTC.getHour();
    b = RTC.getMinute();
    printDigit(2, b / 10, false);
    printDigit(3, b % 10, (a >= 12));
    if (a > 12 && !twenty_four) a -= 12;
    printDigit(0, a / 10, false);
    printDigit(1, a % 10, true);
  }
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

