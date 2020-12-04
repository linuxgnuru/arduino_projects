#include <NS_energyShield.h>
#include <Wire.h>

#define BATT_CAP 1200 // mAh

NS_energyShield eS; // Create NS-energyShield object called "eS"

long lastTime = millis();
unsigned long TimeToEmpty;
int Current[10], AvgCurr;
boolean charging;

const int lowbatPin = LED_BUILTIN;

void setup()
{
  Serial.begin(9600);
  pinMode(lowbatPin, OUTPUT);
}

void loop()
{
  if (millis() > lastTime + 100)
    TTE();
  if (millis() % 5000 == 0)
  {
    if (!charging)
    {
      Serial.print(F("int: "));
      Serial.print(TimeToEmpty / 60);
      Serial.print(F(":"));
      Serial.println(TimeToEmpty % 60);
      digitalWrite(lowbatPin,
                   (((TimeToEmpty / 60) <= 0) && ((TimeToEmpty % 60) <= 5)));
    }
    delay(1); // Ensure that a ms passes, so it does not double print
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

