#include <SPI.h>

#define LATCH 10

byte data[] = {
  0b00000000, 0b00000000
};

const int del = 250;
int aORb = 0;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
  SPI.transfer(data[0]);
  SPI.transfer(data[1]);
  digitalWrite(LATCH, HIGH);
  while (!Serial)
    ;
  Serial.println(F("Enter LED num (0-15)"));
}

void loop()
{
  int lednum = 0;
  
  while (Serial.available() > 0)
  {
    lednum = Serial.parseInt();
    if (Serial.read() == '\n')
    {
      if (lednum < 0 || lednum > 15)
      {
        Serial.println(F("\nInvalid number"));
      }
      else
      {
        lednum = constrain(lednum, 0, 15);
        makeStuff(lednum);
        Serial.print(F("led: "));
        Serial.println(lednum);
        Serial.print(F("0: ["));
        Serial.print(data[0], BIN);
        Serial.print(F("] 1: ["));
        Serial.print(data[1], BIN);
        Serial.println(F("]\n"));
        digitalWrite(LATCH, LOW);
        SPI.transfer(data[0]);
        SPI.transfer(data[1]);
        digitalWrite(LATCH, HIGH);
      }
      Serial.println(F("Enter LED num (0-15)"));
    }
  }
}

void demo()
{
  for (int i = 0; i < 2; i++)
  {
    for (int j = 7; j > -1; j--)
    {
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
      bitClear(data[i], j);
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(LATCH, HIGH);
      delay(del);
    }
  }
}

void makeStuff(int lednum)
{
  if (lednum < 0 || lednum > 15)
    return;
  if (lednum < 8)
  {
    bitWrite(data[0], lednum, !bitRead(data[0], lednum));
  }
  else
  {
    lednum = abs(lednum - 8);
    bitWrite(data[1], lednum, !bitRead(data[1], lednum));
  }
}

