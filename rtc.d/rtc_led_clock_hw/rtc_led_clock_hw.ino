#include <Wire.h>
#include <RTClib.h>
#include <pcf8574.h>

#define DEBUG 1

#ifdef SOFT
RTC_Millis RTC;
#else
RTC_PCF8563 RTC;
#endif
PCF8574 PCF_20(0x20);

int cur_hour;
int cur_min;
int last_hour;
int i;

void setup()
{
//    Serial.begin(9600);
    Wire.begin();
#ifdef SOFT
    RTC.begin(DateTime(__DATE, __TIME));
#else
    RTC.begin();
#endif
    for (i = 0; i < 14; i++)
      pinMode(i, OUTPUT);
    // Set the analog pins for digital output as well
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    // following line sets the RTC to the date & time this sketch was compiled
//    RTC.adjust(DateTime(__DATE__, __TIME__));
    // loop through each light to test that they all work.
    last_hour = 14;
    for (i = 0; i < 8; i++)
      PCF_20.write(i, 1);
    for (i = 0; i < 14; i++)
      digitalWrite(i, LOW);
    // do a simple test for all the LEDs
    digitalWrite(LED_BUILTIN, HIGH);
    for (i = 0; i < 13; i++)
    {
      digitalWrite(i, HIGH);
      switch (i)
      {
        case 0:  PCF_20.write(0, 0); delay(100); PCF_20.write(0, 1); break;
        case 1: PCF_20.write(6, 0); delay(100); PCF_20.write(6, 1); break;
        case 2: PCF_20.write(5, 0); delay(100); PCF_20.write(5, 1); break;
        case 3: digitalWrite(12, HIGH); delay(100); digitalWrite(12, LOW); break;
        case 4: PCF_20.write(4, 0); delay(100); PCF_20.write(4, 1); break;
        case 5: PCF_20.write(3, 0); delay(100); PCF_20.write(3, 1); break;
        case 6: digitalWrite(A3, HIGH); delay(100); digitalWrite(A3, LOW); break;
        case 7: digitalWrite(A2, HIGH); delay(100); digitalWrite(A2, LOW); break;
        case 8: digitalWrite(A1, HIGH); delay(100); digitalWrite(A1, LOW); break;
        case 9: PCF_20.write(2, 0); delay(100); PCF_20.write(2, 1); break;
        case 10: PCF_20.write(1, 0); delay(100); PCF_20.write(1, 1); break;
        case 11: digitalWrite(A0, HIGH); delay(100); digitalWrite(A0, LOW); break;
      }
      digitalWrite(i, LOW);
    }
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    DateTime now = RTC.now();
    cur_hour = now.hour();
    cur_min = now.minute();
    if (last_hour == 14)
      last_hour = cur_hour;
    // first, check AM or PM
    digitalWrite(LED_BUILTIN, (cur_hour >= 12 ? HIGH : LOW));
    // HOUR
    // change 24 hour into normal hour
    cur_hour = (cur_hour > 12 || cur_hour == 0 ? abs(cur_hour - 12) : cur_hour);
    // turn off the previous hour.
    if (cur_hour != last_hour)
    {
      if (last_hour == 1)
        last_hour = 12;
      digitalWrite(last_hour - 1, LOW);
      last_hour = cur_hour;
    }
    digitalWrite(cur_hour - 1, HIGH);
    // MINUTES
    // 0
    if (cur_min > 57 || cur_min <= 2)
    {
      digitalWrite(A0, LOW);
      PCF_20.write(0, 0);
    }
    // 5
    if (cur_min > 2 && cur_min <= 7)
    {
      PCF_20.write(0, 1);
      PCF_20.write(6, 0); // 5
    }
    // 10
    if (cur_min > 7 && cur_min <= 12)
    {
      PCF_20.write(6, 1);
      PCF_20.write(5, 0); // 10
    }
    // 15
    if (cur_min > 12 && cur_min <= 17)
    {
      PCF_20.write(5, 1);
      digitalWrite(12, HIGH);
    }
    // 20
    if (cur_min > 17 && cur_min <= 22)
    {
      digitalWrite(12, LOW);
      PCF_20.write(4, 0);
    }
    // 25
    if (cur_min > 22 && cur_min <= 27)
    {
      PCF_20.write(4, 1);
      PCF_20.write(3, 0); // 25
    }
    // 30
    if (cur_min > 27 && cur_min <= 32)
    {
      PCF_20.write(3, 1);
      digitalWrite(A3, HIGH);
    }
    // 35
    if (cur_min > 32 && cur_min <= 37)
    {
      digitalWrite(A3, LOW);
      digitalWrite(A2, HIGH);
    }
    // 40
    if (cur_min > 37 && cur_min <= 42)
    {
      digitalWrite(A2, LOW);
      digitalWrite(A1, HIGH);
    }
    // 45
    if (cur_min > 42 && cur_min <= 47)
    {
      digitalWrite(A1, LOW);
      PCF_20.write(2, 0);
    }
    // 50
    if (cur_min > 47 && cur_min <= 52)
    {
      PCF_20.write(2, 1);
      PCF_20.write(1, 0);
    }
    // 55
    if (cur_min > 52 && cur_min <= 57)
    {
      PCF_20.write(1, 1);
      digitalWrite(A0, HIGH);
    }
    delay(1000);
}

    /*
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(" since 2000 = ");
    Serial.print(now.get());
    Serial.print("s = ");
    Serial.print(now.get() / 86400L);
    Serial.println("d");
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.get() + 7 * 86400L + 30);
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    Serial.println();
    */

// The following is for LED digits
/*
static const int m_tensPins[] = { LED_BUILTIN, 12, 11, 10, 9, 8, 7 };

static const int m_onesPins[] = { 6, 5 ,4, 3, 2, 1, 0 };

static const int m_tens[10][7] = {
				{ 13, 12, 11, 14,  9,  8,  7 }, // 0
				{ 14, 14, 11, 14, 14,  8, 14 }, // 1
				{ 13, 14, 11, 10,  9, 14,  7 }, // 2
				{ 13, 14, 11, 10, 14,  8,  7 }, // 3
				{ 14, 12, 11, 10, 14,  8, 14 }, // 4
				{ 13, 12, 14, 10, 14,  8,  7 }, // 5
				{ 13, 12, 14, 10,  9,  8,  7 }, // 6
				{ 13, 14, 11, 14, 14,  8, 14 }, // 7
				{ 13, 12, 11, 10,  9,  8,  7 }, // 8
				{ 13, 12, 11, 10, 14,  8,  7 }  // 9
				};

static const int m_ones[10][7] = {
				{  6,  5,  4, 14,  2,  1,  0 }, // 0
				{ 14, 14,  4, 14, 14,  1, 14 }, // 1
				{  6, 14,  4,  3,  2, 14,  0 }, // 2
				{  6, 14,  4,  3, 14,  1,  0 }, // 3
				{ 14,  5,  4,  3, 14,  1, 14 }, // 4
				{  6,  5, 14,  3, 14,  1,  0 }, // 5
				{  6,  5, 14,  3,  2,  1,  0 }, // 6
				{  6, 14,  4, 14, 14,  1, 14 }, // 7
				{  6,  5,  4,  3,  2,  1,  0 }, // 8
				{  6,  5,  4,  3, 14,  1,  0 }  // 9
				};

int h_ones;
int m_tens;
int m_ones;

*/

