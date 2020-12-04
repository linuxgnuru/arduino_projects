/**********************************************************/
/*  ReadAll - energyShield by NightShade Electronics V1.0 */
/*                                                        */
/*  This sketch reads voltage, current, percent           */
/*  charge, and temperature from an energyShield by       */
/*  NightShade Electronics.                               */
/*                                                        */ 
/*  Created by Aaron D. Liebold                           */
/*  on August 11, 2014                                    */
/*                                                        */
/*  This code is released into the public domain.         */
/**********************************************************/

#include <NS_energyShield.h>
#include <Wire.h>

// Global Variables
int Voltage, Current, AdapterVoltage;
float Percent, Temperature;

// Defines NS_energyShield object called "eS"
NS_energyShield eS; 

void setup()
{
  // eS.address(B00110110); // Can be used if address is changed from default (Advanced)
  for (int i = 2; i < 6; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  Serial.begin(9600);
  while(!Serial); // Wait for open COM port
  delay(5000);

  Serial.println("Voltage (mV)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (mV)"); // Prints header
}

void loop()
{
  Voltage = eS.voltage(); // Voltage is returned in mV
  Current = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent()/2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature()/8; // Temperature is returned in 0.125 C increments
  AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage 
  if (Percent < 30)
    digitalWrite(8, HIGH);
  // Print out results
  Serial.print(Voltage);
  Serial.print(" mV\t\t");
  Serial.print(Current);
  Serial.print(" mA\t\t");
  Serial.print(Percent,1);
  Serial.print(" %\t\t");
  Serial.print(Temperature,2);
  Serial.print(" C\t\t");
  Serial.print(AdapterVoltage);
  Serial.println(" mV");
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, LOW);
    delay(250);
  }
  for (int i = 5; i > 1; i--)
  {
    digitalWrite(i, HIGH);
    delay(250);
  }
  for (int i = 5; i > 1; i--)
  {
    digitalWrite(i, LOW);
    delay(250);
  }
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, HIGH);
    delay(250);
  }
//  delay(1000); // Wait 1 second
}

/*
void blinky()
{
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, LOW);
    delay(250);
  }
  for (int i = 5; i > 1; i--)
  {
    digitalWrite(i, HIGH);
    delay(250);
  }
  for (int i = 5; i > 1; i--)
  {
    digitalWrite(i, LOW);
    delay(250);
  }
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, HIGH);
    delay(250);
  }
}
*/

