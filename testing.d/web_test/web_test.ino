#include <SPI.h>

const byte LATCH = 10;

unsigned long lastM = 0;

byte data = 0b00000000;

void setup()
{
  Serial.begin(9600);
  pinMode(LATCH, OUTPUT);
  SPI.begin();
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b10000000);
  digitalWrite(LATCH, HIGH);
}

void loop()
{
  unsigned long curM = millis();
  if (curM - lastM >= 1000)
  {
    lastM = curM;
    for (int ctr = 0; ctr < 8; ctr++)
    {
      //bitSet(data, !bitRead(data, ctr));
      bitSet(data, ctr);
      printLed();
    }
    for (int ctr = 7; ctr > -1 ; ctr--)
    {
      //bitSet(data, !bitRead(data, ctr));
      bitClear(data, ctr);
      printLed();
    }
  }
  Serial.print(F("data "));
  Serial.println(data, BIN);
}

void printLed()
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(data);
  digitalWrite(LATCH, HIGH);
}

