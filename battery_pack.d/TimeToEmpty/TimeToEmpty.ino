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

#define BATT_CAP 1200 // mAh

NS_energyShield eS; // Create NS-energyShield object called "eS"

// Global variables
long lastTime = millis(); 
unsigned long TimeToEmpty;
int Current[10];
int AvgCurr;
boolean charging = false;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  // Run TTE every 100 ms
  if (millis() > lastTime + 100)
    TTE();
  // Print results every 5 seconds
  if (millis() % 5000 == 0)
  {
    if (!charging) 
    {
      Serial.print("Time to Empty: ");      
      Serial.print(TimeToEmpty / 60); // Hours
      Serial.print(" hr ");
      Serial.print(TimeToEmpty % 60); // Minutes
      Serial.println(" min");
    }
    else
      Serial.println("Charging!");
    delay(1); // Ensure that a ms passes, so it does not double print
  }
}

void TTE()
{
  for (int i = 9; i > 0; i--)
    Current[i] = Current [i-1];
  Current[0] = eS.current();
  for (int i = 9; i >= 0; i--)
  {
    if (Current[i] < 0)
    {
      Current[i] = -Current[i];
      charging = 0;
    }
    else
      charging = 1;
  }
  // Check that energyShield is not charging
  for (int i = 0; i < 10; i++)
    AvgCurr += Current[i];
  AvgCurr /= 10;
  // Minutes until empty
  TimeToEmpty = (unsigned long) BATT_CAP * eS.percent() * 60 / AvgCurr / 200;
  lastTime = millis();
}

