#include <NS_energyShield.h>
#include <Wire.h>

#define BATT_CAP 1200 // mAh

NS_energyShield eS; // Create NS-energyShield object called "eS"

long lastTime = millis();
unsigned long TimeToEmpty;
int Current[10], AvgCurr;
boolean charging;

const int chargePin = A3;
const int ledPin = A2;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 500;

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(chargePin, OUTPUT);
  //Serial.begin(115200);
}

void loop()
{
  if (millis() > lastTime + 100)
    TTE();
  if (millis() % 5000 == 0)
  {
    if (!charging)
    {
        digitalWrite(chargePin, LOW);
      if (((TimeToEmpty / 60) <= 0) && ((TimeToEmpty % 60) <= 5))
      {
        // BLINK
        Blink();
      }
      /*
        Serial.print("Time to Empty: ");
        Serial.print(TimeToEmpty / 60); // Hours
        Serial.print(" hr ");
        Serial.print(TimeToEmpty % 60); // Minutes
        Serial.println(" min");
      */
    }
    else
      digitalWrite(chargePin, HIGH);
      //Serial.println("Charging!");
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

void Blink()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
