#include <Wire.h>
#include <RTClib.h>
#include <pcf8574.h>

RTC_PCF8563 RTC;
PCF8574 PCF_20(0x20);  // minutes

int my_hour;
int my_min;
int last_hour;
int i;

void setup()
{
//    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
    for (i = 0; i < 14; i++)
      pinMode(i, OUTPUT);
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    // loop through each light to test that they all work.
    last_hour = 14;
    for (i = 13; i > 0; i--)
    {
      digitalWrite(i, HIGH);
      delay(100);
    }
    for (i = 0; i < 8; i++)
    {
      PCF_20.write(i, 0);
      delay(100);
    }
    delay(500);
    for (i = 1; i < 14; i++)
    {
      digitalWrite(i, LOW);
      delay(100);
    }
    for (i = 7; i > -1; i--)
    {
      PCF_20.write(i, 1);
      delay(100);
    }
}

void loop()
{
    DateTime now = RTC.now();
    my_hour = now.hour();
    my_min = now.minute();
    if (last_hour == 14) last_hour = my_hour;
    // first, check AM or PM
    digitalWrite(LED_BUILTIN, (my_hour > 12 ? HIGH : LOW));
    // HOUR
    // change 24 hour into normal hour
    my_hour = (my_hour > 12 || my_hour == 0 ? abs(my_hour - 12) : my_hour);
    // turn off the previous hour.
    if (my_hour != last_hour)
    {
      digitalWrite(abs(last_hour - 12) + 1, LOW);
      last_hour = my_hour;
    }
/*
    Serial.print("my_hour: ");
    Serial.print(my_hour);
    Serial.print("  last_hour: ");
    Serial.print(last_hour);
    Serial.print("  l_h pin: ");
    Serial.println(abs(last_hour - 12) + 1);
*/
/*
    abs_hour = abs(my_hour - 12);
    if (abs_hour == 0) abs_hour = 12;
    digitalWrite(abs_hour, LOW);
*/
    digitalWrite(abs(my_hour - 12) + 1, HIGH);
    //Serial.println(my_hour);
//    Serial.println(abs(my_hour - 12) + 1);
//    Serial.println(my_min);
    // MINUTES
    // 0
    if (my_min == 0 || my_min < 5)
    {
      for (i = 0; i < 8; i++)
        PCF_20.write(i, 1);
    }
    // 5
    if (my_min > 4 && my_min < 10)
    {
      // don't need to turn off any LED since they were already turned off earlier
      PCF_20.write(0, 0); // 5
    }
    // 10
    if (my_min > 10 && my_min < 15)
    {
      PCF_20.write(0, 1); // turn off last light; 5
      PCF_20.write(1, 0); // 10
    }
    // 15
    if (my_min > 14 && my_min < 20)
    {
      PCF_20.write(1, 0);
      PCF_20.write(2, 0);
    }
    // 20
    if (my_min > 20 && my_min < 25)
    {
      PCF_20.write(1, 1);
      PCF_20.write(2, 0); // 20
    }
    // 25
    if (my_min > 24 && my_min < 30)
    {
      PCF_20.write(2, 1);
      PCF_20.write(3, 0); // 25
    }
    // 30
    if (my_min > 29 && my_min < 35)
    {
      PCF_20.write(4, 0);
      PCF_20.write(3, 0);
    }
    // 35
    if (my_min > 34 && my_min < 40)
    {
      PCF_20.write(4, 0);
      PCF_20.write(3, 1);
    }
    // 40
    if (my_min > 39 && my_min < 45)
    {
      PCF_20.write(4, 1);
      PCF_20.write(5, 0); // 40
    }
    // 45
    if (my_min > 44 && my_min < 50)
    {
      PCF_20.write(5, 0);
      PCF_20.write(6, 0);
    }
    // 50
    if (my_min > 49 && my_min < 55)
    {
      PCF_20.write(6, 0);
      PCF_20.write(5, 1);
    }
    // 55
    if (my_min > 54 && my_min != 0)
    {
      PCF_20.write(6, 1);
      PCF_20.write(7, 0); // 55
    }
    delay(3000);
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

