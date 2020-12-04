#include <SPI.h>

#define NUM_CHIPS 3
#define MAX_DELAY 250

const byte LATCH = 10;

byte data[NUM_CHIPS];

int myDelay = 500;

unsigned int prevMillis = 0;
boolean sequence1Done = false;
boolean sequence2Done = false;
int ctr_a = 0;
int ctr_b = 0;


boolean goingRight = true;
boolean prevRight = false;
boolean justSet = false;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  for (int i = 0; i < NUM_CHIPS; i++)
    data[i] = 0b00000000;
}

void loop()
{
  if (sequence1Done == false)
  {
    if ((millis() - prevMillis >= myDelay) && sequence2Done == false)
    {
      prevMillis += myDelay;
      toggle(ctr_a);
      if (goingRight)
      {
        if (ctr_a++ >= NUM_CHIPS * 8)
        {
          // end second "for" loop
          sequence2Done = true;
          ctr_a = 0;
        }
      }
      else
      {
        if (ctr_a-- <= 0)
        {
          // end second "for" loop
          sequence2Done = true;
          ctr_a = (NUM_CHIPS * 8) - 1;
        }
      }
    }
    if (sequence2Done == true)
    {
      if (ctr_b++ >= 2)
      {
        sequence2Done = false;
        sequence1Done = true;
        if (prevRight != goingRight)
        {
          goingRight = !goingRight;
          prevRight = goingRight;
        }
      } // end first "for" loop
    }
  }
  else
  {
    sequence1Done = false;
    ctr_b = 0;
  }
}

void toggle(int input)
{
  int chip, pin;

  myDelay = map(analogRead(A0), 0, 1023, 0, MAX_DELAY);
  // TODO make the down to just a couple of lines
  if (input < 8)
  {
    chip = NUM_CHIPS - 1;
    pin = input;
  }
  else if (input > 7 && input < 16)
  {
    chip = NUM_CHIPS - 2;
    pin = input - 8;
  }
  else if (input > 15 && input < 24)
  {
    chip = NUM_CHIPS - 3;
    pin = input - 16;
  }
  else if (input > 23 && input < 32)
  {
    chip = NUM_CHIPS - 4;
    pin = input - 24;
  }
  bitWrite(data[chip], pin, !bitRead(data[chip], pin));
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < NUM_CHIPS; i++)
    SPI.transfer(data[i]);
  digitalWrite(LATCH, HIGH);
}

