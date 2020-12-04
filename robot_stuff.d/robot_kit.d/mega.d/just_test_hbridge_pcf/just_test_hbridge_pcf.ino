#include <PCF8574.h>
#include <Wire.h>

#define DEBUG

#include <DebugUtils.h>

#define MOTOR_A 0
#define MOTOR_B 1

PCF8574 pcf20(0x20);

const byte runningOKLed = 4; // led - green
const byte lowBatIntLed = 5; // led - yellow
const byte lowBatExtLed = 6; // led - red
const byte backingUpLed = 7; // led - blue


unsigned int lastMillis = 0;

typedef enum {
  EN,
  IN_A,
  IN_B
} pin_enum;

// to use this method:  mPins[MOTOR_RIGHT][EN]
const byte mPins[2][3] = {
  // EN  IN_A IN_B
  {  6, A1,  A3 }, // motor 0 (right)
  {  5,  4,   3 }  // motor 1 (left)
};

boolean emergStop = false;

// --- IMPORTANT variables / flags ---

//boolean backup_back, backup_left, backup_right, backup_back_left;

const int speed = 180;

boolean reverse[2] = {
  false, // motor A left
  false  // motor B right
};

void setup()
{
  for (int i = 0; i < 3; i++)
  {
    pinMode(mPins[MOTOR_A][i], OUTPUT);
    pinMode(mPins[MOTOR_B][i], OUTPUT);
  }
  pcf20.begin();
  Serial.begin(9600);
  pcf20.write(runningOKLed, LOW);
  pinMode(8, INPUT);
  pinMode(10, INPUT);
  pinMode(12, INPUT);
}

void loop()
{
  #if 0
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillis >= 1000) // every 1 second
  {
    // blink
    pcf20.write(runningOKLed, !pcf20.read(runningOKLed));
  }
  #endif
  DEBUG_PRINTFLN("emergStop", emergStop);
  if (!digitalRead(12)) // stop
  {
    emergStop = true;
  }
  if (!emergStop)
  {
    if (!digitalRead(10)) // left
    {
      reverse[MOTOR_A] = !reverse[MOTOR_A];
    }
    if (!digitalRead(8)) // right
    {
      reverse[MOTOR_B] = !reverse[MOTOR_B];
    }
    setMotor(MOTOR_A, speed);
    setMotor(MOTOR_B, speed);
  }
  //delay(500);
}

void setMotor(int motorNum, int speed)
{
  if (emergStop)
  {
    digitalWrite(mPins[0][EN], LOW);
    digitalWrite(mPins[1][EN], LOW);
    digitalWrite(mPins[0][IN_A], LOW);
    digitalWrite(mPins[1][IN_B], LOW);
  }
  else
  {
    analogWrite(mPins[motorNum][EN], speed);
    digitalWrite(mPins[motorNum][(!motorNum ? IN_B : IN_A)], reverse[motorNum]);
    digitalWrite(mPins[motorNum][IN_B], !reverse[motorNum]);
  }
}





#if 0
// TODO / FIXME the following isn't working correctly for some reason...
// both front and back sensors say there's nothing under them or every sensor is triggered
void blinkLeds()
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillisBlinkBlue >= 100)
  {
    lastMillisBlinkBlue = currentMillis;
    DEBUG_PRINTLN("blink blue");
    pcf20.write(backingUpLed, !pcf20.read(backingUpLed));
    //    pcf20.write(runningOKLed, !pcf20.read(runningOKLed));
  }
}

void blink_extBat()
{
  unsigned int currentMillis = millis();
  // only blink for 3 minutes
  //if (currentMillis - lastMillisBlinkKill >= 180000)
  if (currentMillis - lastMillisBlinkKill >= 30000)
  {
    lastMillisBlinkKill = currentMillis;
    pcf20.write(lowBatExtLed, LOW);
  }
  else // don't blink the led if it's been over 3 minutes
  {
    if (currentMillis - lastMillisBlink >= 1000)
    {
      lastMillisBlink = currentMillis;
      pcf20.write(lowBatExtLed, !pcf20.read(lowBatExtLed));
    }
  }
}
#endif

