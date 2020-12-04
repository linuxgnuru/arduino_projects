// Example 54.2 - PCF8563 square-wave generator (signal from pin 7)

#include "Wire.h"
#define PCF8563address 0x51

void PCF8563oscOFF()
// turns off oscillator
{
  Wire.beginTransmission(PCF8563address);
  Wire.write(0x0D);
  Wire.write(0);
  Wire.endTransmission();
}

void PCF8563osc1Hz()
// sets oscillator to 1 Hz
{
  Wire.beginTransmission(PCF8563address);
  Wire.write(0x0D);
  Wire.write(B10000011);
  Wire.endTransmission();
}

void PCF8563osc32Hz()
// sets oscillator to 32 kHz
{
  Wire.beginTransmission(PCF8563address);
  Wire.write(0x0D);
  Wire.write(B10000010);
  Wire.endTransmission();
}

void PCF8563osc1024kHz()
// sets oscillator to 1.024 kHz
{
  Wire.beginTransmission(PCF8563address);
  Wire.write(0x0D);
  Wire.write(B10000001);
  Wire.endTransmission();
}

void PCF8563osc32768kHz()
// sets oscillator to 32.768 kHz
{
  Wire.beginTransmission(PCF8563address);
  Wire.write(0x0D);
  Wire.write(B10000000);
  Wire.endTransmission();
}

void setup()
{
  Wire.begin();
}

void loop()
{
  PCF8563osc1Hz();
  delay(2000);
  PCF8563osc32Hz();
  delay(2000);
  PCF8563osc1024kHz();
  delay(2000);
  PCF8563osc32768kHz();
  delay(2000);
  PCF8563oscOFF();
  delay(2000);
}
