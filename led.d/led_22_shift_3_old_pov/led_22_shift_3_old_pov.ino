#include <SPI.h>

// change the following based on how many shift register chips you are using
//#define BITNUM 24
#define CHIPNUM 3

#define MYDELAYA 10
#define MYDELAYB 8

int spd = MYDELAYA;
const int LATCH = 10;

byte data[CHIPNUM];

void setup()
{
  for (int i = 0; i < CHIPNUM; i++)
    data[i] = 0b00000000;
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, HIGH); // don't send any data
}

void loop()
{
  spd = map(analogRead(A2), 0, 1023, 5, 35);
  for (int i = CHIPNUM; i > -1; i--)
  {
    for (int l = 0; l < 8; l++)
    {
      bitSet(data[i], l);
      digitalWrite(LATCH, LOW);
      for (int c = 0; c < CHIPNUM; c++)
        SPI.transfer(data[c]);
      digitalWrite(LATCH, HIGH);
      delay(spd);
    }
#ifndef DASH
  }
  for (int i = CHIPNUM; i > -1; i--)
  {
#endif
    for (int l = 0; l < 8; l++)
    {
      bitClear(data[i], l);
      digitalWrite(LATCH, LOW);
      for (int c = 0; c < CHIPNUM; c++)
        SPI.transfer(data[c]);
      digitalWrite(LATCH, HIGH);
      if (l != 7)
        delay(spd - 4);
    }
  }
  for (int i = 0; i < CHIPNUM; i++)
  {
    for (int l = 7; l > -1; l--)
    {
      bitSet(data[i], l);
      digitalWrite(LATCH, LOW);
      for (int c = 0; c < CHIPNUM; c++)
        SPI.transfer(data[c]);
      digitalWrite(LATCH, HIGH);
      delay(spd);
    }
#ifndef DASH
  }
  for (int i = 0; i < CHIPNUM; i++)
  {
#endif
    for (int l = 7; l > -1; l--)
    {
      bitClear(data[i], l);
      digitalWrite(LATCH, LOW);
      for (int c = 0; c < CHIPNUM; c++)
        SPI.transfer(data[c]);
      digitalWrite(LATCH, HIGH);
      if (l != 0)
        delay(spd - 4);
    }
  }
}

