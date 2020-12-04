
//    FILE: PCF8574_int.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.00
// PURPOSE:
//    DATE: 14-12-2013
//     URL:
//
// Released to the public domain

#include <PCF8574.h>
#include <Wire.h>

PCF8574 pcf20(0x20);

unsigned long  lastTime = 0;
volatile unsigned long count[8];

void setup()
{
  Serial.begin(115200);
  Serial.println("\nTEST PCF8574 Interrupt\n");
  //Serial.println("\nTEST PCF8574\n");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  attachInterrupt(0, catchChange, CHANGE);
}

void loop()
{
  // once per second
  if (millis() - lastTime >= 1000)
  {
    lastTime += 1000;/*
    uint8_t value = PCF_38.read8();
    Serial.print("#38:\t");
    Serial.println(value, BIN);*/

    for (uint8_t i = 0; i < 8; i++)
    {
      Serial.print(count[i]);
      Serial.print(',');
    }
    Serial.println();

  }
}

void catchChange()
{
  static uint8_t last = 0xFF;
  uint8_t tmp = TWBR;
  TWBR = 2; // speed up I2C;
  uint8_t value = pcf20.read8();
  TWBR = tmp;
  // which pins are changed since last time from LOW to HIGH
  uint8_t v = (last ^ value) & value;
  last = value;
  uint8_t mask = 0x01;
  for (uint8_t i = 0; i < 8; i++)
  {
    if (v & mask)
      count[i]++;
    mask <<= 1;
  }
}

