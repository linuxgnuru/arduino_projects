//boolean random_mode = false;
/*
  void random_stuff()
  {
  int rn0, rn1;

  rn0 = random(0, 15);
  rn1 = random(0, 15);
  led(rn0, LED_ON, BOARD_0);
  led(rn1, LED_ON, BOARD_1);
  delay(RDL);
  led(rn0, LED_OFF, BOARD_0);
  led(rn1, LED_OFF, BOARD_1);
  }
*/

#if 0
#include <SPI.h>

#define MX 24
#define MIN_ 500
#define MAX_ 1020

const byte LATCH = 10;
const byte anaPin = A1;

byte d[] = {
  0b00000000, 0b00000000, 0b00000000
};

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  writeData();
}

void loop()
{
  int pin, chip;
  int v = map(analogRead(anaPin), MIN_, MAX_, 0, MX);
  int nv;

  for (int i = v; i < MX; i++)
  {
    pin = i % 8;
    chip = abs(i / 8 - 2);
    bitClear(d[chip], pin);
  }
  if (v != 0)
  {
    if (v == MX - 1)
      nv = v + 1;
    else
      nv = v;
    for (int i = 0; i < nv; i++)
    {
      pin = i % 8;
      chip = abs(i / 8 - 2);
      bitSet(d[chip], pin);
    }
  }
  writeData();
}

void writeData()
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(d[0]);
  SPI.transfer(d[1]);
  SPI.transfer(d[2]);
  digitalWrite(LATCH, HIGH);
}

