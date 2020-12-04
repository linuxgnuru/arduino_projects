#include <Hbridge_l293d.h>

Hbridge_l293d myBridge(5, 8, 2, 6, 3, 7);

boolean MOTOR_LEFT = myBridge.MOTOR0;
boolean MOTOR_RIGHT = myBridge.MOTOR1;

unsigned int lastMillisTurnLeft = 0;
unsigned int lastMillisTurnRight = 0;
unsigned int lastMillisStop = 0;

const byte rPin1 = 9;
const byte rPin2 = 10;

void setup()
{
  Serial.begin(9600);
  pinMode(rPin1, INPUT_PULLUP);
  pinMode(rPin2, INPUT_PULLUP);
  //myBridge.SetDirection(MOTOR_LEFT, myBridge.MOTOR_FORWARD);
  //myBridge.SetDirection(MOTOR_RIGHT, myBridge.MOTOR_FORWARD);
}

void loop()
{
  unsigned int cMillis = millis();
  if (!myBridge.IsStopped())
  {
    if (!myBridge.StartMotor(MOTOR_LEFT, 255))
      Serial.println(F("error starting left motor"));
    if (!myBridge.StartMotor(MOTOR_RIGHT, 255))
      Serial.println(F("error starting right motor"));
  }

  if (digitalRead(rPin1) == LOW && digitalRead(rPin2) == LOW)
  {
    if (!myBridge.StartMotor(MOTOR_LEFT, 255))
      Serial.println(F("error starting left motor"));
    if (!myBridge.StartMotor(MOTOR_RIGHT, 255))
      Serial.println(F("error starting right motor"));
  }
#if 0
  if (digitalRead(rPin1) == LOW && !myBridge.IsStopped())
  {
    myBridge.SetDirection(MOTOR_LEFT, (myBridge.GetDirection(MOTOR_LEFT) == myBridge.MOTOR_FORWARD ? myBridge.MOTOR_REVERSE : myBridge.MOTOR_FORWARD));
  }
  if (digitalRead(rPin2) == LOW && !myBridge.IsStopped())
  {
    myBridge.SetDirection(MOTOR_RIGHT, (myBridge.GetDirection(MOTOR_RIGHT) == myBridge.MOTOR_FORWARD ? myBridge.MOTOR_REVERSE : myBridge.MOTOR_FORWARD));
  }
#endif
  //  #if 0
  if (cMillis - lastMillisTurnLeft >= 3000)
  {
    lastMillisTurnLeft = cMillis;
    //if ((myBridge.GetDirection(MOTOR_LEFT) != myBridge.MOTOR_STOP) && (myBridge.GetDirection(MOTOR_RIGHT) != myBridge.MOTOR_STOP))
    myBridge.SetDirection(MOTOR_LEFT, (myBridge.GetDirection(MOTOR_LEFT) == myBridge.MOTOR_FORWARD ? myBridge.MOTOR_REVERSE : myBridge.MOTOR_FORWARD));
    //myBridge.SetDirection(MOTOR_RIGHT, (myBridge.GetDirection(MOTOR_RIGHT) == myBridge.MOTOR_FORWARD ? myBridge.MOTOR_REVERSE : myBridge.MOTOR_FORWARD));
  }
  if (cMillis - lastMillisTurnRight >= 2000)
  {
    lastMillisTurnRight = cMillis;
    myBridge.SetDirection(MOTOR_RIGHT, (myBridge.GetDirection(MOTOR_RIGHT) == myBridge.MOTOR_FORWARD ? myBridge.MOTOR_REVERSE : myBridge.MOTOR_FORWARD));
  }
  //  #endif
  if (cMillis - lastMillisStop >= 20000)
  {
    lastMillisStop = cMillis;
    myBridge.StopAllMotors();
  }
}

