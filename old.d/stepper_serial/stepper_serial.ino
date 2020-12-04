#include <Stepper.h>

const int stepsPerRevolution = 4076;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

int stepCount = 0;         // number of steps the motor has taken

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
}

int stopCtr = 2038;

void loop()
{
  stepCount = 0;
  // step one step:
  while (stepCount < stopCtr)
  {
    myStepper.step(1);
    if (Serial.available())
    {
      while (Serial.available() > 0)
      {
        stopCtr = Serial.read();
      }
    }
    Serial.print("steps:" );
    Serial.println(stepCount);
    stepCount++;
    //  delay(250);
  }
}

