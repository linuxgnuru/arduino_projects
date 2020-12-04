/**********************************************************/
/*  TimeToEmpty V1.0                                      */
/*  for the energyShield by NightShade Electronics        */
/*                                                        */
/*  This sketch calculates the time until the battery     */
/*  will be empty and the current rate of discharge.      */
/*                                                        */
/*  Created by Aaron D. Liebold                           */
/*  on August 11, 2014                                    */
/*                                                        */
/*  This code is released into the public domain.         */
/**********************************************************/

#include <NS_energyShield.h>
#include <Wire.h>
#include <LiquidCrystal.h>

#define BATTERY_CAPACITY 1200 // mAh

NS_energyShield eS; // Create NS-energyShield object called "eS"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Global variables
long lastTime = millis();
unsigned long TimeToEmpty;
int Current[10], AvgCurrent;
boolean charging;
int Amps;
float Percent, Temperature;
boolean ch = false;

void setup()
{
  lcd.begin(16, 2);
  //  Serial.begin(9600);
}

void loop()
{
  Amps = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature() / 8; // Temperature is returned in 0.125 C increments

  if (millis() > lastTime + 100)
    TTE(); // Run TTE every 100 ms

  if (millis() % 5000 == 0) // Print results every 5 seconds
  {
    if (!charging)
    {
      if (ch)
      {
        lcd.setCursor(0, 0);
        for (int i = 0; i < 16; i++)
          lcd.print(" ");
        ch = false;
      }
      lcd.setCursor(0, 0);
      // Hours
      lcd.print(TimeToEmpty / 60);
      lcd.print(":");
      // Minutes
      lcd.print(TimeToEmpty % 60);
      lcd.print(" ");
      lcd.print(Percent, 1);
      lcd.print("% ");
      lcd.setCursor(0, 1);
      if (Amps < 0)
      {
        lcd.print(abs(Amps));
        lcd.print("mA ");
      }
      lcd.print(Temperature, 2);
      lcd.print("C ");

      /*
        Serial.print("Time to Empty: ");
        Serial.print(TimeToEmpty/60); // Hours
        Serial.print(" hr ");
        Serial.print(TimeToEmpty%60); // Minutes
        Serial.println(" min");
      */
    }
    else
    {
      ch = true;
      lcd.setCursor(0, 0);
      lcd.print("Charging!   ");
      lcd.setCursor(0, 1);
      for (int i = 0; i < 16; i++)
        lcd.print(" ");
      //Serial.print("Charging!");
    }
    // Ensure that a ms passes, so it does not double print
    delay(1);
  }
}

void TTE()
{
  for (int i = 9; i > 0; i--)
    Current[i] = Current [i - 1];
  Current[0] = eS.current();

  for (int i = 9; i >= 0; i--)
  {
    if (Current[i] < 0)
    {
      Current[i] = -Current[i];
      charging = false;
    }
    else
      charging = true;
  }
  // Check that energyShield is not charging
  for (int i = 0; i < 10; i++)
    AvgCurrent += Current[i];
  AvgCurrent /= 10;
  // Minutes until empty
  TimeToEmpty = (unsigned long) BATTERY_CAPACITY * eS.percent() * 60 / AvgCurrent / 200;
  lastTime = millis();
}

