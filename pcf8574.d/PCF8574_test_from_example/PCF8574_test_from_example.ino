//
//    FILE: PCF8574_test.ino
//  AUTHOR: Rob Tillaart
//    DATE: 7-febr-2013
//
// PUPROSE: test PCF8574 library
//

#include "PCF8574.h"
#include <Wire.h>

PCF8574 PCF_01(0x20);

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
  Serial.begin(9600);
  Serial.print(F("PCF8574_test version: "));
  Serial.println(PCF8574_LIB_VERSION);

  PCF_01.begin();

  int x = PCF_01.read8();
  Serial.print(F("Read "));
  Serial.println(x, BIN);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  inputString.reserve(200);

  Serial.println(F("Enter pin number (0-7)"));
  delay(1000);
}

void loop()
{
//  int num;
  Serial.println(F("HLT"));
  while (Serial.available() == 0);
  /*{
    int inChar = Serial.read();
    if (isDigit(inChar))
      inputString += (char)inChar;
    if (inChar == '\n')
    {
      Serial.println(inputString);
      num = inputString.toInt();
//      digitalWrite(num, (digitalRead(num) ? LOW : HIGH));
      inputString = "";
    }
    switch (num)*/
    switch (Serial.read())
    {
      case 'H': case 'h': doHigh(); break;
      case 'L': case 'l': doLow(); break;
      case 'T': case 't': doToggle(); break;
    }
  //}
}

void doHigh()
{
  PCF_01.write(4, HIGH);
  int x = PCF_01.read8();
  Serial.print(F("Read "));
  Serial.println(x, BIN);
}

void doLow()
{
  PCF_01.write(4, LOW);
  int x = PCF_01.read8();
  Serial.print(F("Read "));
  Serial.println(x, BIN);
}

void doToggle()
{
  PCF_01.toggle(4);
  int x = PCF_01.read8();
  Serial.print(F("Read "));
  Serial.println(x, BIN);
}

