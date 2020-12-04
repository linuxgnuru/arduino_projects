#include <MyDelay.h>

MyDelay md(1000);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  if (md.check())
  {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  if (md.getDelay() == 1000)
    md.setDelay(500);
}

