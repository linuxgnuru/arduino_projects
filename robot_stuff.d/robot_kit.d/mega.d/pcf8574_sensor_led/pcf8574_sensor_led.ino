#include <PCF8574.h>
#include <Wire.h>

PCF8574 pcf20(0x20);

const byte forwDnSensorPin = 0;
const byte backDnSensorPin = 1;
const byte backUpSensorPin = 2;
const byte runningOKLed = 3;
const byte lowBatIntLed = 4;
const byte lowBatExtLed = 5;
const byte backingUpLed = 6;

unsigned int fdMillis;
unsigned int bdMillis;
unsigned int buMillis;

void setup()
{
  Serial.begin(115200);
  pcf20.begin();
  //pinMode(onboardLed, OUTPUT);
  //  attachInterrupt(0, catchChange, CHANGE);
}

void loop()
{
  static bool state = false;
  unsigned int currentMillis = millis();

  //Limit button read to 20 times a second
  //Fast enough for most buttons
  //but this way you don't have a dimmer output because it's blanked during button read
  //a read takes 460us t 16Mhz Arduino and normal I2C speed.
  if (currentMillis - fdMillis >= 50)
  {
    fdMillis = currentMillis;
    if (state != pcf20.readButton(frontDownSensorPin))
    {
      if (state)
      {
        //toggle the LED
        digitalWrite(onboardLed, !digitalRead(onboardLed));
        Serial.println(pcf20.read8(), BIN);
      }
      state = !state;
    }
  }
}

