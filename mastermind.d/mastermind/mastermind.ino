#include <Wire.h>
#include <pcf8574.h>

PCF8574 PCF_20(0x20);

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
//
//    FILE: pcf8574_test.ino
//  AUTHOR: Rob Tillaart
//    DATE: 27-08-2013
//
// PUPROSE: demo 
//

#include "PCF8574.h"
#include <Wire.h>

// adjust addresses if needed
PCF8574 PCF_38(0x38);  // add switches to lines  (used as input)
PCF8574 PCF_39(0x39);  // add leds to lines      (used as output)

void setup()
{
  Serial.begin(115200);
  Serial.println("\nTEST PCF8574\n");

  uint8_t value = PCF_38.read8();
  Serial.print("#38:\t");
  Serial.println(value);

  for (int i=0; i<255; i++)
  {
    PCF_39.write8(i);
    delay(100);
  }

  PCF_39.write(0, 1);
  for (int i=0; i<7; i++)
  {
    PCF_39.shiftLeft();
    delay(100);
  }

  for (int i=0; i<7; i++)
  {
    PCF_39.shiftRight();
    delay(100);
  }

  for (int i=0; i<8; i++)
  {
    PCF_39.write(i, 1);
    delay(100);
    PCF_39.write(i, 0);
    delay(100);
  }

  for (int i=0; i<8; i++)
  {
    PCF_39.toggle(i);
    delay(100);
    PCF_39.toggle(i);
    delay(100);
  }
}

void loop()
{
  // echos the lines
  uint8_t value = PCF_38.read8();
  PCF_39.write8(value);
  delay(100);
}
//
// END OF FILE
//
*/
