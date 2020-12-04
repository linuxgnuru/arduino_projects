#include <Wire.h>

unsigned long lastMillis = 0;
#define SLAVE_ADDRESS 0x04

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
    //Serial.print(F("Scanning. . . "));
    Serial.println(F("Scanning..."));
    nDevices = 0;
    //for (address = 1; address < 127; address++ )
    for (address = 4; address < 127; address++ )
    {
      /*
      Serial.print(F(". "));
      if (address > 50 && !once)
      {
        once = true;
        Serial.println();
      }
      */
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      digitalWrite(LED_BUILTIN, HIGH);
      Wire.beginTransmission(address);
      Wire.write(0);
      error = Wire.endTransmission();
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print(F("address: "));
      Serial.println(address);
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

