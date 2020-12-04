#include <LDR.h>
#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1 10     // IN1 on the ULN2003 driver 1
#define motorPin2  9     // IN2 on the ULN2003 driver 1
#define motorPin3  8     // IN3 on the ULN2003 driver 1
#define motorPin4  7     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

unsigned long lastMillis = 0;

const int waitDelay = 1000;

int num = 0;

void setup()
{
  Serial.begin(115200);
  //pinMode(iPin, INPUT_PULLUP);
  //pinMode(Pin, OUTPUT);
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(140.0);
  stepper.setSpeed(700);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  num = map(, 0, 59, 0, 4076);
  //if (stepper.distanceToGo() != 0) { }
  stepper.moveTo(num);
  stepper.run();

}
/*
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= waitDelay)
  {
    lastMillis = currentMillis;
    for (int i = A0; i < A8; i++)
    {
      Serial.print(analogRead(i));
      Serial.print(F(" "));
    }
    Serial.println();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

*/
