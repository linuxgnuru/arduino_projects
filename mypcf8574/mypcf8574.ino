#include <pcf8574.h>
#include <Wire.h>

PCF8574 PCF_20(0x20);  // add switches to lines  (used as input)

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
}

void loop()
{

  digitalWrite(LED_BUILTIN, (PCF_20.read(0) == 0))
  digitalWrite(7, !digitalRead(7));
  delay(500);
}

//
// END OF FILE
//
/*
  uint8_t value = PCF_38.read8();
  for (int i=0; i<8; i++)
  {
    PCF_39.toggle(i);
    delay(100);
    PCF_39.toggle(i);
    delay(100);
  }
  for (int i=0; i<8; i++)
  {
    PCF_20.write(i, 1);
    delay(100);
    PCF_20.write(i, 0);
    delay(100);
  }
  // echos the lines
  uint8_t value = PCF_38.read8();
  PCF_39.write8(value);
  */

