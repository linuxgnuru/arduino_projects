#include <SPI.h>

const byte LATCH = 10;

void setup()
{
  SPI.begin();
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
}  // end of setup

byte c;

void loop()
{  
  digitalWrite(LATCH, LOW);
  SPI.transfer(c);
  digitalWrite(LATCH, HIGH);
  c++;
  delay(20);
}  // end of loop
