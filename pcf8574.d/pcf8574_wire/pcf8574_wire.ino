#include <Wire.h>

#define expander B00100000  //expander address

volatile byte count = 0;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  expanderWrite(B11111111);
  attachInterrupt(0, expanderInterrupt, LOW);
}

void loop()
{
  if (count = 1)
  {
    Serial.println(expanderRead(), BIN);
    count = 0;
    attachInterrupt(0, expanderInterrupt, LOW);
  }
}

void expanderInterrupt()
{
  detachInterrupt(0);
  count = 1;
}

void expanderWrite(byte _data)
{
  Wire.beginTransmission(expander);
  Wire.write(_data);
  Wire.endTransmission();
}

byte expanderRead()
{
  byte _data;
  Wire.requestFrom(expander, 1);
  if (Wire.available())
  {
    _data = Wire.read();
  }
  return _data;
}

