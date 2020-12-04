#include <SPI.h>

const byte LATCH = 10;

byte data = 0b00000000;

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
}

void loop()
{
  for (int i = 7; i > -1; i--)
  {
    bitSet(data, i);
    digitalWrite(LATCH, LOW);
    SPI.transfer(data);
    digitalWrite(LATCH, HIGH);
    delay(40);
  }
  for (int i = 7; i > -1; i--)
  {
    bitClear(data, i);
    digitalWrite(LATCH, LOW);
    SPI.transfer(data);
    digitalWrite(LATCH, HIGH);
    delay(40);
  }
  for (int i = 0; i < 8; i++)
  {
    bitSet(data, i);
    digitalWrite(LATCH, LOW);
    SPI.transfer(data);
    digitalWrite(LATCH, HIGH);
    delay(40);
  }
  for (int i = 0; i < 8; i++)
  {
    bitClear(data, i);
    digitalWrite(LATCH, LOW);
    SPI.transfer(data);
    digitalWrite(LATCH, HIGH);
    delay(40);
  }
}

