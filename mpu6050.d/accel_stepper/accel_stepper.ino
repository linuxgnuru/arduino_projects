#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1  3     // IN1 on the ULN2003 driver 1
#define motorPin2  4     // IN2 on the ULN2003 driver 1
#define motorPin3  5     // IN3 on the ULN2003 driver 1
#define motorPin4  6     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

int curD = 22;

int dest = 0;

int stops[] = {
  1019, 0, -1019, 0
};

void setup()
{
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(100.0);
  stepper.setSpeed(200);
  stepper.moveTo(dest);
  //Serial.begin(115200);
}//--(end setup )---

void loop()
{
  stepper.runToNewPosition(4076/2);
//    stepper.runToNewPosition(map(analogRead(A0), 0, 1023, 0, 4076));
  /*
    long var = analogRead(A0);
    int pos = map(var, 0, 1023, 0, 4076);
    stepper.runToNewPosition(pos);
    */
//  stepper.runToNewPosition(-1019);
/*
  stepper.runToNewPosition(0);
  stepper.runToNewPosition(2038);
  stepper.runToNewPosition(-2038);
*/
//#if 0
  //Change direction when the stepper reaches the target position
/*
  if (curD != stops[dest])
  {
    curD = stops[dest];
    Serial.print("stops[dest]: ");
    Serial.println(curD);
  }
  */
  if (stepper.distanceToGo() == 0)
  {
//    stepper.moveTo(-stepper.currentPosition());
//    Serial.print("stops[dest]: ");
//    Serial.println(stops[dest]);
    stepper.moveTo(stops[dest]);
    dest++;
    if (dest > 3)
      dest = 0;
  }
  stepper.run();
//#endif
}
