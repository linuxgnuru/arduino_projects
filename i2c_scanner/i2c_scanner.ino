// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>

unsigned long lastMillis = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  // Leonardo: wait for serial monitor
  while (!Serial)
    ;
  Serial.println(F("\nI2C Scanner"));
}

boolean once = false;

void loop()
{
  byte error, address;
  int nDevices;
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= 5000)
  {
    lastMillis = currentMillis;
    Serial.print(F("Scanning. . . "));
    nDevices = 0;
    for (address = 1; address < 127; address++ )
    {
      Serial.print(F(". "));
      if (address > 50 && !once)
      {
        once = true;
        Serial.println();
      }
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      digitalWrite(LED_BUILTIN, HIGH);
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      digitalWrite(LED_BUILTIN, LOW);
      if (error == 0)
      {
        Serial.print(F("I2C device found at address [0x"));
        if (address < 16)
          Serial.print(F("0"));
        Serial.print(address, HEX);
        Serial.println(F("]"));
        nDevices++;
      }
      else if (error == 4)
      {
        Serial.print(F("Unknown error at address [0x"));
        if (address < 16)
          Serial.print(F("0"));
        Serial.print(address, HEX);
        Serial.println(F("]"));
      }
    }
    once = false;
    if (nDevices == 0)
      Serial.println(F("No I2C devices found\n"));
    else
      Serial.println(F("done\n"));
  }
}

