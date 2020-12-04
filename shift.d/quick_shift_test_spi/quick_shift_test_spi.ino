#include <SPI.h>

const byte LATCH = 10;

byte data[] = {
  0b00000000, 0b00000000, 0b00000000
};

void setup()
{
  SPI.begin();
}

void loop()
{
  for (int i = 0; i < 20; i++)
  {
    digWrite(i, HIGH);
    delay(100);
    digWrite(i, LOW);
  }
}

void digWrite(int pin, int val)
{
  if (pin > -1 && pin < 8)
  {
    if (val == HIGH) bitSet(data[0], pin);
    else bitClear(data[0], pin);
  }
  else if (pin > 7 && pin < 16)
  {
    if (val == HIGH) bitSet(data[1], abs(pin - 8));
    else bitClear(data[1], abs(pin - 8));
  }
  else if (pin > 15 && pin < 20)
  {
    if (val == HIGH) bitSet(data[2], abs(pin - 16));
    else bitClear(data[2], abs(pin - 16));
  }
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < 3; i++)
    SPI.transfer(data[i]);
  digitalWrite(LATCH, HIGH);
}

