/*
  4 digit (via shift registers) temperature
 
 sample code:
 
 shiftOut(dataPin, clockPin, digitArray[k - 48]); // char k = '5';
 shiftOut(dataPin, clockPin, alphabetArray[c - 65]);  // char c = 'A' ... 'Z'
 
 */
#include <Wire.h>
#include <RTClib.h>

RTC_PCF8563 RTC;

#include <ctype.h>
#include <string.h>

//#define DEBUG 1

const int mdyPin = 12;

const int dataPin  = 4; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 2; // Pin connected to SH_CP of 74HC595 - yellow wire

const int rtcVccPin = A2;
const int rtcGndPin = A3;

const int ledVccPin = 6;
const int ledGndPin = 7;

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
/*
  ---     A
 |   |  F   B
  ---     G
 |   |  E   C
  ---     D    dp
 
*/

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
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(rtcGndPin, OUTPUT);
  pinMode(rtcVccPin, OUTPUT);
  pinMode(ledVccPin, OUTPUT);
  pinMode(ledGndPin, OUTPUT);
  digitalWrite(rtcGndPin, LOW);
  digitalWrite(rtcVccPin, HIGH);
  digitalWrite(ledGndPin, LOW);
  digitalWrite(ledVccPin, HIGH);
  Wire.begin();
  RTC.begin();
  // following line sets the RTC to the date & time this sketch was compiled
//  RTC.adjust(DateTime(__DATE__, __TIME__));
  /*
  for (int i = 0; i < 26; i++)
   {
   printChar(0, alphabetArray[i]);
   delay(500);
   }
   */
  //printDigit(0, 1, false);
  /*
  for (int i = 0; i < 4; i++)
   {
   for (int j = 0; j < 10; j++)
   {
   printDigit(i, j, false);
   delay(250);
   }
   }
   for (int i = 0; i < 4; i++)
   printDigit(i, 99, false);
   */
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
    /*
    Serial.print(now.year()
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
*/
    /*
#ifdef DEBUG
   if (Serial.available())
   {
   int val = Serial.read() - 48;
   printDigit(0, val, false);
   }
   #endif
   */
  //  getTemp();
  //  digitalWrite(LED_BUILTIN, (cfFlag == FAR ? HIGH : LOW));
  delay(1000);
}

#if 0
void getTemp()
{
  float tempC, tempF;
  float curTemp;
  float decTmp, dec;
  int tens, ones, tenth, hundreth;
  int reading;
  float raw;
  tempC = (float)raw / 16.0;
  tempF = tempC * 1.8 + 32.0;
  reading = digitalRead(cfPin);
  if (reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
        cfFlag = (cfFlag == FAR ? CEL : FAR);
    }
  }
  lastButtonState = reading;
  curTemp = (cfFlag == CEL ? tempC : tempF);
  tens = (int)curTemp / 10;
  ones = (int)curTemp % 10;
  decTmp = (curTemp - ((float)tens * 10) - (float)ones); // for decimal
  dec = decTmp * 100;
  tenth = (int)dec / 10;
  hundreth = (int)dec % 10;
  data[0] = digitArray[tens];
  data[1] = digitArray[ones];
  bitSet(data[2], 2);
  data[2] = digitArray[tenth];
  data[3] = digitArray[hundreth];
  //  blink_dp(3, 250);
  printDigit(0, tens, false);
  printDigit(1, ones, true);
  printDigit(2, tenth, false);
  printDigit(3, hundreth, false);
}
#endif

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

void blink_dp(int pos, int d)
{
  if (pos < 4)
  {
    byte myData = data[pos];
    bitSet(myData, 2);
    delay(d);
    bitClear(myData, 2);
  }
}

void clearD(int pos)
{
  int ctr = 0;
  
  data[pos] = 0b00000000;
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  digitalWrite(latchPin, 1);
}

