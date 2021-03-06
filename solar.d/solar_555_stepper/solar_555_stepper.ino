#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

#define LED_PIN LED_BUILTIN // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

//const int leftPin = 8;
//const int rightPin = 9;

const int leftLedPin = 7;
const int rightLedPin = 10;
const int noLedPin = 11;
int pos = 90;   // initial position
int sens1 = A0; // LRD 1 pin
int sens2 = A1; // ---------------------------------------------------------------------------------------------------------------LDR 2 pin
int tolerance = 2;
int dest = 0;
boolean left1 = false;
boolean right1 = false;

void setup()
{
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(200);
  pinMode(leftLedPin, OUTPUT);
  pinMode(rightLedPin, OUTPUT);
  pinMode(noLedPin, OUTPUT);
  //pinMode(leftPin, INPUT_PULLUP);
  //pinMode(rightPin, INPUT_PULLUP);
  //  stepper.moveTo(0);
  //  pinMode(sens1, INPUT);
  //  pinMode(sens2, INPUT);
  delay(2000); // a 2 seconds delay while we position the solar panel
}

void loop()
{
  int val1 = analogRead(sens1); // read the value of sensor 1
  int val2 = analogRead(sens2); // read the value of sensor 2
  //int leftVal = digitalRead(leftPin);
  //int rightVal = digitalRead(rightPin);

  if ((abs(val1 - val2) <= tolerance) || (abs(val2 - val1) <= tolerance))
    //if ((leftVal == HIGH && rightVal == HIGH) || (leftVal == LOW && rightVal == LOW))
  {
    digitalWrite(noLedPin, HIGH);
    digitalWrite(leftLedPin, LOW);
    digitalWrite(rightLedPin, LOW);
    left1 = right1 = false;
    //do nothing if the difference between values is within the tolerance limit
  }
  else
  {
    if (val1 > val2)
      //if (rightVal == LOW && leftVal == HIGH)
    {
      if (!left1)
      {
        digitalWrite(leftLedPin, HIGH);
        digitalWrite(rightLedPin, LOW);
        digitalWrite(noLedPin, LOW);
        right1 = false;
        left1 = true;
      }
      pos = --pos;
    }
    if (val1 < val2)
      //if (rightVal == HIGH && leftVal == LOW)
    {
      if (!right1)
      {
        digitalWrite(leftLedPin, LOW);
        digitalWrite(rightLedPin, HIGH);
        digitalWrite(noLedPin, LOW);
        right1 = true;
        left1 = false;
      }
      pos = ++pos;
    }
  }
  if (pos > 180)
  {
    pos = 180;  // reset to 180 if it goes higher
  }
  if (pos < 0)
  {
    pos = 0;  // reset to 0 if it goes lower
  }
  //            x = ypr[0] * 180/M_PI;
  //            y = ypr[1] * 180/M_PI;
  //            x = map(x, -72, 65, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  //            y = map(y, -83, 41, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
  dest = map(pos, 0, 180, -1019, 1019);
  stepper.moveTo(dest);
  //    if (stepper.distanceToGo() == 0) {}
  stepper.run();
  //    stepper.runToNewPosition(pos);
  //    pos = map(var, 0, 1023, 0, 4076);
  //    stepper.runToNewPosition(pos);
  //  myservo.write(pos); // write the position to servo
  delay(50);
}
