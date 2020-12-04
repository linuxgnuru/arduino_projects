/**
    @file
    @Author Septillion (https://github.com/sseptillion)
    @date 2016-05-20
    @brief Example how to use buttonRead()

    Because of the quasi-bidirectional port on the PCF8574 it's
    possible to use the same pin as input and output at the same time.

    For this, connect the LED (or other device) between Vcc and the pin and
    a button between GND and the pin.

    In order to read the pin the output is shortly (460us @ normal I2C speed)
    set high during the read. So only use devices that can handle the small break
    in state. It's not visable for the human eye with a LED.

    buttonRead() can also be used for pins that are input only.

    In this example pin 0 of the PCF8574 (address 0x20) is used to blink a
    LED but at the same time it can be used to toggle the onboard LED of
    the Arduino.

    NOTE: The button will affect the LED on the same pin. The led will light up
    no matter the set output state when you press the button.
*/

#include <PCF8574.h>
#include <Wire.h>

PCF8574 pcf20(0x20);

const byte onboardLed = LED_BUILTIN;
const byte PcfLedPin = 0;
const byte PcfButtonPin = 1;

unsigned int blinkMillis;
unsigned int buttonMillis;
/*
unsigned long  lastTime = 0;
volatile unsigned long count[8];
*/
void setup()
{
  Serial.begin(115200);
  pcf20.begin();
  pinMode(onboardLed, OUTPUT);
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
  if (currentMillis - buttonMillis >= 50)
  {
    buttonMillis = currentMillis;
    if (state != pcf20.readButton(PcfButtonPin))
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
#if 0
  //Lets blink the same output
  if (currentMillis - blinkMillis >= 500)
  {
    //Update time
    blinkMillis = currentMillis;
    pcf20.toggle(PcfLedPin);
    Serial.println(pcf20.read8(), BIN);
    /*
     for (uint8_t i = 0; i < 8; i++)
      {
      Serial.print(count[i]);
      Serial.print(',');
      }
      Serial.println();*/
  }
}

void catchChange()
{
  static uint8_t last = 0xFF;
  uint8_t tmp = TWBR;
  TWBR = 2; // speed up I2C;
  uint8_t value = pcf20.read8();
  TWBR = tmp;
  // which pins are changed since last time from LOW to HIGH
  uint8_t v = (last ^ value) & value;
  last = value;
  uint8_t mask = 0x01;
  for (uint8_t i = 0; i < 8; i++)
  {
    if (v & mask)
      count[i]++;
    mask <<= 1;
  }
}

#endif
