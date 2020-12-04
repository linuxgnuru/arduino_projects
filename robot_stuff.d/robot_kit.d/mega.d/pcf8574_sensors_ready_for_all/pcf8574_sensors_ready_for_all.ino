#include <PCF8574.h>
#include <Wire.h>

#define ALL_OK    0
#define BOT_FRONT 1
#define BOT_BACK  2
#define TOP_BACK  4

PCF8574 pcf20(0x20);

const byte frontDownSensorPin = 0;
const byte backDownSensorPin  = 1;
const byte backUpSensorPin  = 2;

const byte runningOKLed = 3;
const byte lowBatIntLed = 4;
const byte lowBatExtLed = 5;
const byte backingUpLed = 6;

unsigned int lastMillis;

volatile boolean hasChanged = false;

boolean hasChanged_fd = false;
boolean hasChanged_bd = false;
boolean hasChanged_bu = false;

void setup()
{
  pcf20.begin();
  attachInterrupt(0, catchChange, FALLING);
}

void loop()
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillis >= 50)
  {
    lastMillis = currentMillis;
    if (hasChanged)
    {
      hasChanged_fd = !pcf20.readButton(frontDownSensorPin);
      hasChanged_bd = !pcf20.readButton(backDownSensorPin);
      hasChanged_bu = !pcf20.readButton(backUpSensorPin);
      if (hasChanged_fd || hasChanged_bd || hasChanged_bu)
      {
        if (hasChanged_fd)
        {
          //pcf20.write(lowBatIntLed, LOW);
        }
        if (hasChanged_bd)
        {
          //pcf20.write(lowBatExtLed, LOW);
        }
        if (hasChanged_bu)
        {
          //pcf20.write(backingUpLed, LOW);
        }
        pcf20.write(runningOKLed, HIGH);
      }
      else
      {
        pcf20.write(runningOKLed, LOW);
        pcf20.write(lowBatIntLed, HIGH);
        pcf20.write(lowBatExtLed, HIGH);
        pcf20.write(backingUpLed, HIGH);
      }
      hasChanged = false;
    }
  }
}

void catchChange()
{
  hasChanged = true;
}

