#ifndef MYDELAY_H
#define MYDELAY_H

#include "Arduino.h"

class MyDelay
{
  private:
    int targetTime = -1;
    unsigned long lastMillis;

  public:
    MyDelay();
    MyDelay(int t);
    boolean check();
    void setDelay(int t);
    int getDelay();
};
/*
  Serial.print(str);
  Serial.println(str);
  Serial.print(str); Serial.print(F(": ")); Serial.print(val);
  Serial.print(str); Serial.print(F(": ")); Serial.println(val);
  Serial.print(str, BIN);
  Serial.println(str, BIN);
  Serial.print(str, DEC);
  Serial.println(str, DEC);
  */
#endif
