#include "LDR.h"

LDR::LDR(boolean useLed)
{
  use_led = useLed;
  lowest = 1024;
  lowestP = 8;
  if (use_led)
    pinMode(checkPin, INPUT_PULLUP);
  for (int i = 0; i < 8; i++)
  {
    if (use_led)
      pinMode(ledPins[i], OUTPUT);
    data[i] = 1024;
  }
}

int LDR::getDir()
{
  int ret;

    for (int i = A0; i < A7 + 1; i++)
    {
      ar = i - 14;
      data[ar] = analogRead(i);
    }
    for (int i = 0; i < 8; i++)
    {
      if (data[i] <= MAX_THRESHOLD && data[i] < lowest)
      {
        lowest = data[i];
        lowestP = i;
      }
    }
    if (lowestP != 8 && lowest != 1024)
    {
      if (data[lowestP] <= MAX_THRESHOLD)
      {
        best = lowestP;
        //printDir(best, true);
//        sprintf(str, "%d", best);
//        Serial.write(str, 4);
        if (use_led && digitalRead(checkPin) == HIGH)
        {
          for (int i = 0; i < 8; i++)
            digitalWrite(ledPins[i], (lowestP == i));
        }
      }
    }
    if (use_led && lowest == 1024)
    {
      for (int i = 0; i < 8; i++)
        digitalWrite(ledPins[i], LOW);
    }
    lowest = 1024;
    lowestP = 8;
    return best;
}

void LDR::ledDemo()
{
  if (use_led)
  {
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(ledPins[i], HIGH);
        delay(100);
        digitalWrite(ledPins[i], LOW);
        delay(100);
    }
  }
}

void LDR::printDir()
{
  switch (best)
  {
    case -1: break;
    case FORWARD: Serial.println(F("FORWARD")); break;
    case FORWARD_LEFT: Serial.println(F("FORWARD_LEFT")); break;
    case LEFT: Serial.println(F("LEFT")); break;
    case BACKWARD_LEFT: Serial.println(F("BACKWARD_LEFT")); break;
    case BACKWARD: Serial.println(F("BACKWARD")); break;
    case BACKWARD_RIGHT: Serial.println(F("BACKWARD_RIGHT")); break;
    case RIGHT: Serial.println(F("RIGHT")); break;
    case FORWARD_RIGHT: Serial.println(F("FORWARD_RIGHT")); break;
    case STOP: Serial.println(F("STOP")); break;
    default: Serial.print(F("ERROR ")); Serial.println(best); break;
  }
}
