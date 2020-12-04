#include <Wire.h>
#include <i2ckeypad.h>

#define ROWS 4
#define COLS 4

// With A0, A1 and A2 of PCF8574 to ground I2C address is 0x38
#define PCF8574_ADDR 0x20


i2ckeypad kpd = i2ckeypad(PCF8574_ADDR, ROWS, COLS);

void setup()
{
  Serial.begin(9600);

  Wire.begin();

  kpd.init();

  Serial.print("Testing keypad/PCF8574 I2C port expander arduino lib\n\n");
}

void loop()
{
  char key = kpd.get_key();

  if(key != '\0') {
        Serial.print(key);
  }
}

