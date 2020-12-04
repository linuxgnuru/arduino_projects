#include <LDR.h>

LDR ldr(false);

int dir;

void setup()
{
   Serial.begin(9600);
}

void loop()
{
  dir = ldr.getDir();
  ldr.printDir();
  delay(1000);
}

