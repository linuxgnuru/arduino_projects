#include <Wire.h>
#include <RTClib.h>
#include <pcf8574.h>

RTC_PCF8563 RTC;
PCF8574 PCF_20(0x20);

int my_hour;
int my_min;
int last_hour = 14;
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
    if (last_hour == 14) last_hour = my_hour;
    my_min = now.minute();
    // first, check AM or PM
    digitalWrite(LED_BUILTIN, (my_hour > 12 ? HIGH : LOW));
    // TODO
    // For some reason, the previous hour isn't changing from HIGH to LOW when the new hour comes.
    // HOUR
    // change 24 hour into normal hour
    my_hour = (my_hour > 12 || my_hour == 0 ? abs(my_hour - 12) : my_hour);
    if (last_hour != my_hour)
    {
      digitalWrite(last_hour, LOW);
      last_hour = my_hour;
    }
    digitalWrite(abs(my_hour - 12) + 1, HIGH);
    // MINUTES
    // 0
    if (my_min == 0)
    {
      for (i = 0; i < 8; i++) PCF_20.write(i, 1);
    }
    // 1 - 5 (5)
    if (my_min > 0 && my_min < 10)
    {
      // don't need to turn off any LED since they were already turned off earlier
      PCF_20.write(0, 0); // 5
    }
    // 6 - 14 (10)
    if (my_min > 5 && my_min < 15)
    {
      PCF_20.write(0, 1); // turn off last light; 5
      PCF_20.write(1, 0); // 10
    }
    // 15
    if (my_min == 15)
    {
      if (PCF_20.read(1) == 1) PCF_20.write(1, 0);
      PCF_20.write(2, 0); // don't turn off 10, just add (turn on) 20
    }
    // 16 - 21 (20)
    if (my_min > 15 && my_min < 21)
    {
      PCF_20.write(1, 1); // turn off last one; 10
      if (PCF_20.read(2) == 1) PCF_20.write(2, 0); // 20
    }
    // 21 - 29 (25)
    if (my_min > 20 && my_min < 30)
    {
      PCF_20.write(2, 1); // turn off last one; 20
      PCF_20.write(3, 0); // 25
    }
    // 30
    if (my_min == 30)
    {
      if (PCF_20.read(3) == 1) PCF_20.write(3, 0);
      PCF_20.write(4, 0);
    }
    // 31 - 34 (35)
    if (my_min > 31 && my_min < 36)
    {
      // if 35 wasn't turned on by 30, turn it on
      if (PCF_20.read(4) == 1) PCF_20.write(4, 0);
      PCF_20.write(3, 1); // turn off 25
    }
    // 36 - 44 (40)
    if (my_min > 35 && my_min < 45)
    {
      PCF_20.write(4, 1); // turn off 35
      PCF_20.write(5, 0); // 40
    }
    // 45
    if (my_min == 45)
    {
      if (PCF_20.read(5) == 1) PCF_20.write(5, 0);
      PCF_20.write(6, 0);
    }
    // 46 - 50 (50)    
    if (my_min > 45 && my_min < 51)
    {
      if (PCF_20.read(6) == 1) PCF_20.write(6, 0);
      PCF_20.write(5, 1); // turn off 40
    }
    // 51 - 60 (55)
    if (my_min > 54 && my_min < 60)
    {
      PCF_20.write(6, 1); // turn off 50
      PCF_20.write(7, 0); // 55
    }
    //     PCF_20.write(0, 1); // 0 = 5
    delay(3000);
    // turn off all the PCF leds
}
