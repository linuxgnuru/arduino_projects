#include <SPI.h>

const byte LATCH = 10;

void setup()
{
  SPI.begin();
}

byte data[] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

int del = 50;

void loop()
{
  for (int i = 7; i > -1; i--)
  {
    for (int j = 0; j < 8; j++)
    {
      bitSet(data[i], j);
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      SPI.transfer(data[2]);
      SPI.transfer(data[3]);
      SPI.transfer(data[4]);
      SPI.transfer(data[5]);
      SPI.transfer(data[6]);
      SPI.transfer(data[7]);
      digitalWrite(LATCH, HIGH);
      delay(del);
    }
  }
  for (int i = 7; i > -1; i--)
  {

    for (int j = 0; j < 8; j++)
    {
      bitClear(data[i], j);
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      SPI.transfer(data[2]);
      SPI.transfer(data[3]);
      SPI.transfer(data[4]);
      SPI.transfer(data[5]);
      SPI.transfer(data[6]);
      SPI.transfer(data[7]);
      digitalWrite(LATCH, HIGH);
      delay(del);
    }
  }
}
