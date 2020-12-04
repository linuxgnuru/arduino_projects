#include <Hbridge_l293d.h>

Hbridge_l293d myBridge();

boolean MOTOR_LEFT = myBridge.MOTOR0;
boolean MOTOR_RIGHT = myBridge.MOTOR1;

unsigned int lastMillisTurnLeft = 0;
unsigned int lastMillisStop = 0;

void setup()
{
  Serial.begin(9600);
  myBridge.SetPins(5, 8, 2);
  myBridge.SetPins(6, 3, 7);
  myBridge.SetDirection(MOTOR_LEFT, myBridge.MOTOR_FORWARD);
  myBridge.SetDirection(MOTOR_RIGHT, myBridge.MOTOR_FORWARD);
  if (!myBridge.StartMotor(MOTOR_LEFT, 255))
    Serial.println(F("error starting left motor"));
  if (!myBridge.StartMotor(MOTOR_RIGHT, 255))
    Serial.println(F("error starting right motor"));
}

void loop()
{
  unsigned int cMillis = millis();
  if (cMillis - lastMillisTurnLeft >= 3000)
  {
    lastMillisTurnLeft = cMillis;
    if (myBridge.GetDirection() != myBridge.MOTOR_STOP)
        myBridge.SetDirection(MOTOR_LEFT, myBridge.MOTOR_BACKWARD);
  }
  if (cMillis - lastMillisStop >= 10000)
  {
    lastMillisStop = cMillis;
    myBridge.StopAllMotors();
  }
}
