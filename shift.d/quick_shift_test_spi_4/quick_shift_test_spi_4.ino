#include <SPI.h>

const byte LATCH = 10;

byte data = 0b00000000;

void setup()
{
  SPI.begin();
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
}

void loop()
{
  for (int s = 0; s < 4; s++)
  {
    for (int j = 0; j < 3; j++)
      digitalWrite(j + 3, HIGH);
    digitalWrite(s + 3, LOW);
    for (int i = 0; i < 8; i++)
    {
      digWrite(i, HIGH);
      delay(25);
      digWrite(i, LOW);
      delay(25);
    }
  }
}

void digWrite(int pin, int val)
{
  if (pin > -1 && pin < 8)
  {
    if (val == HIGH) bitSet(data, pin);
    else bitClear(data, pin);
  }
  digitalWrite(LATCH, LOW);
  SPI.transfer(data);
  digitalWrite(LATCH, HIGH);
}

