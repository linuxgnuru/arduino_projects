#include <SPI.h>

byte Output0, Output1;
byte Check0 = 1;
byte Check1 = 1;

const byte LATCH = 4;

int j;

void setup()
{
  pinMode(13, OUTPUT);//clock
  pinMode(11, OUTPUT);//data
  pinMode(LATCH, OUTPUT);//latch
  pinMode(2, INPUT);//Input from buttons
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  digitalWrite(LATCH, LOW);
  SPI.transfer(255);
  SPI.transfer(255);
  SPI.transfer(0);
  SPI.transfer(0);
  digitalWrite(LATCH, HIGH);
  Serial.begin(9600);
  attachInterrupt(0, pin_read, RISING);
}//setup

void loop()
{
}

void pin_read()
{
  for (j = 0; j < 50; j++)
    delayMicroseconds(1000);
  Check0 = 1;
  Check1 = 1;
  for (j = 0; j < 16; j++)
  {
    digitalWrite(LATCH, LOW);
    SPI.transfer(Check1);
    SPI.transfer(Check0);
    SPI.transfer(Output1);
    SPI.transfer(Output0);
    digitalWrite(LATCH, HIGH);
    delayMicroseconds(500);
    if (digitalRead(2) == HIGH)
    {
      bitWrite((j > 7 ? Output1 : Output0), (j > 7 ? abs(8 - j) : j), 1);
    }//dig check
    else
    {
      bitWrite((j > 7 ? Output1 : Output0), (j > 7 ? abs(8 - j) : j), 0);
    }
    if (j < 8)
      Check0 = Check0 << 1;
    else
      Check1 = Check1 << 1;
  }//j
  digitalWrite(LATCH, LOW);
  SPI.transfer(255);
  SPI.transfer(255);
  SPI.transfer(Output1);
  SPI.transfer(Output0);
  digitalWrite(LATCH, HIGH);
  // while(digitalRead(2)==HIGH) ;//{ }
}//pin_read

