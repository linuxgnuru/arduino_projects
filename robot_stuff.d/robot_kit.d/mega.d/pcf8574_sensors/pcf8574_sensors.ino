#include <PCF8574.h>
#include <Wire.h>

/*
  1, 2, 4 = them selves
  3 = 1 + 2 (front and back)
  5 = 1 + 4 front and top back
  6 = 2 + 4
*/
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

//unsigned int blinkMillis;
//unsigned int fdMillis;
//unsigned int bdMillis;
//unsigned int btMillis;
unsigned int Millis;

volatile boolean hasChanged = false;

boolean hasChanged_fd = false;
boolean hasChanged_bd = false;
boolean hasChanged_bu = false;

void setup()
{
  Serial.begin(115200);
  pcf20.begin();
  //attachInterrupt(0, catchChange, CHANGE);
  attachInterrupt(0, catchChange, FALLING);
}

void loop()
{
  unsigned int currentMillis = millis();

  /* Limit button read to 20 times a second. Fast enough for
     most buttons but this way you don't have a dimmer output
     because it's blanked during button read a read takes
     460us t 16Mhz Arduino and normal I2C speed.
  */
  if (currentMillis - Millis >= 50)
  {
    Millis = currentMillis;
    //Serial.print(F("all pins: "));
    //Serial.println(pcf20.read8(), BIN);

    if (hasChanged)
    {
      hasChanged_fd = !pcf20.readButton(frontDownSensorPin);
      hasChanged_bd = !pcf20.readButton(backDownSensorPin);
      hasChanged_bu = !pcf20.readButton(backUpSensorPin);
      if (hasChanged_fd || hasChanged_bd || hasChanged_bu)
      {
        if (hasChanged_fd)
        {
          Serial.print(F(" fd "));        Serial.print(hasChanged_fd);
          pcf20.write(lowBatIntLed, LOW);
        }
        if (hasChanged_bd)
        {
          Serial.print(F(" bd "));        Serial.print(hasChanged_bd);
          pcf20.write(lowBatExtLed, LOW);
        }
        if (hasChanged_bu)
        {
          Serial.print(F(" bu "));        Serial.print(hasChanged_bu);
          pcf20.write(backingUpLed, LOW);
        }
        pcf20.write(runningOKLed, HIGH);
        Serial.println();
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
  //  sensor_status = ALL_OK;
  hasChanged = true;
  /*
    hasChanged_fd = (!pcf20.readButton(frontDownSensorPin));
    hasChanged_bd = (!pcf20.readButton(backDownSensorPin));
    hasChanged_bu = (!pcf20.readButton(backUpSensorPin));
  */
}

void debugData()
{
  Serial.print(F("front down: "));
  Serial.print(pcf20.readButton(frontDownSensorPin));
  Serial.print(F(" back down: "));
  Serial.print(pcf20.readButton(backDownSensorPin));
  Serial.print(F(" back top: "));
  Serial.println(pcf20.readButton(backUpSensorPin));
}

// old loop
#if 0
pcf_data = pcf20.read8();
if ((pcf_data & (1 << frontDownSensorPin)) &&
    (pcf_data & (1 << backDownSensorPin)) &&
    (pcf_data & (1 << backUpSensorPin)))
{
  sensor_status = ALL_OK;
}

if (last_data != pcf_data)
{
  last_data = pcf_data;
  Serial.print(F("pcf_data: "));
  //Serial.println(pcf_data, BIN);
  Serial.println(pcf20.read8(), BIN);
  if (!(pcf_data & (1 << frontDownSensorPin)))
    sensor_status += BOT_FRONT;
  if (!(pcf_data & (1 << backDownSensorPin)))
    sensor_status += BOT_BACK;
  if (!(pcf_data & (1 << backUpSensorPin)))
    sensor_status += TOP_BACK;
  Serial.print(F("sensor_status: "));
  Serial.println(sensor_status);
  /*
    runningOKLed
    lowBatIntLed
    lowBatExtLed
    backingUpLed
    pcf20.write(pin, on_off);
  */
  switch (sensor_status)
  {
    case 0:
      pcf20.write(runningOKLed, LOW);
      pcf20.write(backingUpLed, HIGH);
      break;
    case 1:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
    case 2:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
    case 3:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
    case 4:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
    case 5:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
    case 6:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
    case 7:
      pcf20.write(runningOKLed, HIGH);
      pcf20.write(backingUpLed, LOW);
      break;
  }
}
delay(200);
#endif

#if 0
if (state != pcf20.readButton(frontDownSensorPin))
{
  if (!state)
  {
    //toggle the LED
    pcf20.toggle(lowBatIntLed);
    //Serial.println(pcf20.read8(), BIN);
  }
}
if (state != pcf20.readButton(backDownSensorPin))
{
  if (!state)
  {
    pcf20.toggle(lowBatExtLed);
  }
  state = !state;
}
if (state != pcf20.readButton(backUpSensorPin))
{
  if (state)
  {
    pcf20.toggle(backingUpLed);
  }
  state = !state;
}
#endif

