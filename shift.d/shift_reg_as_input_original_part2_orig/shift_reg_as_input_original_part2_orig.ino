#include <SPI.h>

byte Output, Check = 1;
int j;

void setup()
{
  pinMode(13, OUTPUT);//clock
  pinMode(11, OUTPUT);//data
  pinMode(4, OUTPUT);//latch
  pinMode(2, INPUT);//Input from buttons
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  SPI.transfer(255);
  SPI.transfer(0);
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  Serial.begin(9600);
  attachInterrupt(0, pin_read, RISING);
}//setup

void loop()
{
}//loop

void pin_read()
{
  for (j = 0; j < 50; j++)
    delayMicroseconds(1000);
  Check = 1;
  for (j = 0; j < 8; j++)
  {
    SPI.transfer(Check);
    SPI.transfer(Output);
    digitalWrite(4, HIGH);
    digitalWrite(4, LOW);
    delayMicroseconds(500);
    if (digitalRead(2) == HIGH)
      bitWrite(Output, j, 1);
    else
      bitWrite(Output, j, 0);
    Check = Check << 1;
  }//j
  SPI.transfer(255);
  SPI.transfer(Output);
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
  // while(digitalRead(2)==HIGH){}
}//pin_read

