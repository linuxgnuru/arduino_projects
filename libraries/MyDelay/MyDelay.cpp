#include "MyDelay.h"

MyDelay::MyDelay() { } 

MyDelay::MyDelay(int t)
{
  targetTime = t;
  lastMillis = 0;
} 

boolean MyDelay::check()
{
  if (targetTime == -1)
    return false;
  unsigned long currentMillis = millis();
  boolean ret = false;

  if (currentMillis - lastMillis >= targetTime)
  {
    lastMillis = currentMillis;
    ret = true;
  }
  return ret;
}

void MyDelay::setDelay(int t)
{
  targetTime = t;
  lastMillis = 0;
}

int MyDelay::getDelay()
{
  return targetTime;
}
