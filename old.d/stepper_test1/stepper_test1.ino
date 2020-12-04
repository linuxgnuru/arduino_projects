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

void setup()
{
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(200);
//  stepper.moveTo(20000);
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
    //            x = ypr[0] * 180/M_PI;
    //            y = ypr[1] * 180/M_PI;
    //            x = map(x, -72, 65, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    //            y = map(y, -83, 41, 0, 180);     // scale it to use it with the servo (value between 0 and 180)
    dest = map(, -90, 90, -1019, 1019);
    stepper.moveTo(dest);
//    if (stepper.distanceToGo() == 0) {}
    stepper.run();
//    stepper.runToNewPosition(pos);
//    pos = map(var, 0, 1023, 0, 4076);
//    stepper.runToNewPosition(pos);
}

