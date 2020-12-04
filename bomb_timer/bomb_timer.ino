#include <SPI.h>

#include "segment_digit.h"

#define NUM_SAMPLES 10

const byte LATCH = 10;

const int rp = 2;
const int yp = 3;
const int gp = 4;
const int dl = 75;

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage

byte data[] = { 0b00000000, 0b00000000 };
int ran = 0;

void setup()
{
  randomSeed(analogRead(A7));
  for (int i = 2; i < 5; i++)
    pinMode(i, OUTPUT);
  pinMode(LATCH, OUTPUT);
  SPI.begin();
}

boolean runme = true;

void loop()
{
  int tens, ones;
  if (runme == true)
  {
    for (int i = 99; i > -1; i--)
    {
      ran = random(0, 7);
      switch (ran)
      {
        case 0:
          digitalWrite(gp, LOW);
          digitalWrite(yp, LOW);
          digitalWrite(rp, LOW);
          break;
        case 1:
          digitalWrite(gp, HIGH);
          digitalWrite(yp, LOW);
          digitalWrite(rp, LOW);
          break;
        case 2:
          digitalWrite(gp, LOW);
          digitalWrite(yp, HIGH);
          digitalWrite(rp, LOW);
          break;
        case 3:
          digitalWrite(gp, LOW);
          digitalWrite(yp, LOW);
          digitalWrite(rp, HIGH);
          break;
        case 4:
          digitalWrite(gp, HIGH);
          digitalWrite(yp, LOW);
          digitalWrite(rp, HIGH);
          break;
        case 5:
          digitalWrite(gp, LOW);
          digitalWrite(yp, HIGH);
          digitalWrite(rp, HIGH);
          break;
        case 6:
          digitalWrite(gp, HIGH);
          digitalWrite(yp, HIGH);
          digitalWrite(rp, HIGH);
          break;
      }
      tens = i / 10;
      ones = i % 10;
      printDigit(1, tens, true);
      printDigit(0, ones, false);
      delay(100);
    }
    runme = false;
  }
  else
  {
    for (int i = 0; i < 15; i++)
    {
      printDigit(1, 8, true);
      printDigit(0, 8, true);
      digitalWrite(gp, HIGH);
      digitalWrite(yp, HIGH);
      digitalWrite(rp, HIGH);
      delay(100);
      printDigit(1, 99, false);
      printDigit(0, 99, false);
      digitalWrite(gp, LOW);
      digitalWrite(yp, LOW);
      digitalWrite(rp, LOW);
      delay(150);
    }
    runme = true;
  }
}

void printDigit(int pos, int num, boolean dp)
{
  int ctr = 0;
  byte myData;
  if (pos < 2 && (num > -1 && num < 10 || num == 99))
  {
    myData = (num == 99 ? 0b00000000 : digitArray[num]);
    if (dp == true)
      bitSet(myData, 7);
    data[pos] = myData;
    digitalWrite(LATCH, LOW);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    digitalWrite(LATCH, HIGH);
  }
}

void clearD(int pos)
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  digitalWrite(LATCH, HIGH);
}

