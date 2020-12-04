#include <SPI.h>

#define LATCH 10

byte data[] = {
  0b00000000, 0b00000000
};

const int del = 250;
int aORb = 0;

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
  SPI.transfer(data[0]);
  SPI.transfer(data[1]);
  digitalWrite(LATCH, HIGH);
}

void loop()
{
  for (int i = 0; i < 2; i++)
  {
    for (int j = 7; j > -1; j--)
    {
      /*
      data[0] = 0b00001111;
      data[1] = 0b11111111;
      */
      bitSet(data[i], j);
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(LATCH, HIGH);
      delay(del);
    }
  }
  for (int i = 0; i < 2; i++)
  {
    for (int j = 7; j > -1; j--)
    {
      bitClear(data[i], j);/*
      data[0] = 0b00000000;
      data[1] = 0b00000000;*/
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(LATCH, HIGH);
      delay(del);
    }
  }
}
