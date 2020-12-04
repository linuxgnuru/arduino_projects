// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// 2010-02-04 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// contributed by @mariusster, see http://forum.jeelabs.net/comment/1902

#include <Wire.h>
#include <RTClib.h>
#include <string.H>

RTC_PCF8563 RTC;

int last_hour = -1;

void setup()
{
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
    pinMode(LED_BUILTIN, OUTPUT);
    // following line sets the RTC to the date & time this sketch was compiled
    digitalWrite(LED_BUILTIN, LOW);
    //RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
    DateTime now = RTC.now();
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
    /*
    if (last_hour == -1)
      last_hour = now.hour;
    else
    {
      if (atoi(now.hour()) != last_hour)
      {
        digitalWrite(LED_BUILTIN, HIGH);
        last_hour = now.hour();
      }
      else
      {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
  */  
    /*
    
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
    delay(3000);
}
