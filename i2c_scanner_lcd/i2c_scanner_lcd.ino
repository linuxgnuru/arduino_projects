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

#include <LiquidCrystal.h>

#include <Wire.h>
// RS, E, 4, 5, 6, 7
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

unsigned long lastMillis = 0;

void setup()
{
  Wire.begin();
  lcd.begin(16, 2);

  //  Serial.begin(9600);
  //  while (!Serial);             // Leonardo: wait for serial monitor
  //  Serial.println("\nI2C Scanner");
  lcd.setCursor(0, 0);
  lcd.print("I2C Scanner");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Scanning for I2C ");
  lcd.setCursor(0, 1);
  lcd.print("Devices...");
  delay(2000);
  lcd.clear();
}

int ctr = 0;

void loop()
{
  int nDevices;
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    //Serial.println("Scanning...");

    nDevices = 0;
    lcd.setCursor(0, 0);
    for (int address = 1; address < 127; address++ )
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      Wire.beginTransmission(address);
      byte error = Wire.endTransmission();
      if (error == 0)
      {
        //for (int i = 0; i < 16; i++)          lcd.print(" ");
        if (ctr++ > 2)
          lcd.setCursor(0, 1);
        lcd.print("0x");
        if (address < 16)
          lcd.print("0");
        lcd.print(address, HEX);
        if (ctr <= 2)
          lcd.print(", ");
        /*
          lcd.setCursor(0, 0);
          lcd.print("I2C device found at ");
          lcd.setCursor(0, 1);
          lcd.print("address 0x");
          //Serial.print("I2C device found at address 0x");
          if (address < 16)
          lcd.print("0");
          //Serial.print("0");
          lcd.print(address, HEX);
          //Serial.print(address, HEX);
          //Serial.println("  !");
        */
        nDevices++;
      }
      else if (error == 4)
      {
        lcd.setCursor(0, 0);
        lcd.print("err 0x");
        if (address < 16)
          lcd.print("0");
        //Serial.print("0");
        lcd.print(address, HEX);
        //Serial.print("Unknown error at address 0x");
        /*
          lcd.setCursor(0, 0);
          lcd.print("Unknown error at");
          lcd.setCursor(0, 1);
          lcd.print(" address 0x");
          if (address < 16)
          lcd.print("0");
          //Serial.print("0");
          lcd.print(address, HEX);
          //Serial.println(address, HEX);
        */
      }
    }
    if (nDevices == 0)
    {
      lcd.setCursor(0, 0);
      lcd.print("No I2C devices");
      lcd.setCursor(0, 1);
      lcd.print("were found");
    }
    ctr = 0;

    //Serial.println("No I2C devices found\n");
    //else      lcd.print("done");
    //Serial.println("done\n");
  }
}

