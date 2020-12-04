//
//    FILE: pcf8574_test.ino
//  AUTHOR: Rob Tillaart
//    DATE: 27-08-2013
//
// PUPROSE: demo 
//

#include <PCF8574.h>
#include <Wire.h>

// adjust addresses if needed
PCF8574 PCF_38(0x38);  // add switches to lines  (used as input)
PCF8574 PCF_20(0x20);  // add leds to lines      (used as output)

int i;

void setup()
{
  //for (i = 0; i < 14; i++)    pinMode(i, OUTPUT);
  Serial.begin(115200);
  Serial.println("\nTEST PCF8574\n");

  uint8_t value = PCF_38.read8();
  Serial.print("#38:\t");
  Serial.println(value);
  for (int i = 0; i < 255; i++)
  {
    PCF_20.write8(i);
    delay(100);
  }
  PCF_20.write(0, 1);
  for (int i = 0; i < 7; i++)
  {
    PCF_20.shiftLeft();
    delay(100);
  }
  for (int i = 0; i < 7; i++)
  {
    PCF_20.shiftRight();
    delay(100);
  }
  for (int i = 0; i < 8; i++)
  {
    PCF_20.write(i, 0);
    delay(100);
    PCF_20.write(i, 1);
    delay(100);
  }
}

void loop()
{
  // echos the lines
//  uint8_t value = PCF_38.read8();
//  PCF_20.write8(value);
  while (1)
  {
    for (int i=0; i<8; i++)
    {
      PCF_20.toggle(i);
      delay(100);
      PCF_20.toggle(i);
      delay(100);
    }
  }
  delay(100);
}

