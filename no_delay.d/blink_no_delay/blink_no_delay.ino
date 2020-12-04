#include <SPI.h>

const unsigned long M = 1000;
unsigned long lastMillis = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  unsigned long currMillis = millis();
  if (currMillis - lastMillis >= M)
  {
    lastMillis = currMillis;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}
