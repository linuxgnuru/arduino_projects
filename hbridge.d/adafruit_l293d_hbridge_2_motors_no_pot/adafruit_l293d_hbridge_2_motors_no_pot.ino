#include "motors.h"

int switchPins[] = {
  6, 7
};

const byte mPins[2][3] = {
  // EN  IN_A IN_B
  { 11, 2, 3 }, // motor 0 right
  { 10, 4, 5 }  // motor 1 left
};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 2; i++)
  {
    pinMode(switchPins[i], INPUT_PULLUP);
    for (int j = 0; j < 3; j++)
      pinMode(mPins[i][j], OUTPUT);
  }
}

void loop()
{
  boolean rev_r = digitalRead(switchPins[S_R]);
  boolean rev_l = digitalRead(switchPins[S_L]);
  if (rev_r && rev_l)
  {
    setMotor(M_L, rev_l);
    setMotor(M_R, rev_r);
  }
  else if (rev_r && !rev_l)
  {
    setMotor(M_L, rev_l);
    stopMotor(M_R);
  }
  else if (rev_l && !rev_r)
  {
    setMotor(M_R, rev_r);
    stopMotor(M_L);
  }
  else if (!rev_l && !rev_r)
  {
    stopMotor(M_L);
    stopMotor(M_R);
  }
}

void setMotor(int motor, boolean reverse)
{
  digitalWrite(mPins[motor][EN], HIGH);
  digitalWrite(mPins[motor][IN_A], !reverse);
  digitalWrite(mPins[motor][IN_B], reverse);
}

void stopMotor(int motor)
{
  digitalWrite(mPins[motor][EN], HIGH);
  digitalWrite(mPins[motor][IN_A], LOW);
  digitalWrite(mPins[motor][IN_B], LOW);
}

