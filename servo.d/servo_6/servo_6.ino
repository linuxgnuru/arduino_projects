/*
   Sweep

   modified version to run 6 servos
*/

#include <Servo.h>

// create servo object to control a servo
Servo myservo[6];
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup()
{
  // attaches the servo on pin 9 to the servo object
  for (int i = 2; i < 8; i++)
    myservo[i - 2].attach(i);
}

void loop()
{
  for (int i = 0; i < 6; i++)
  {
    // goes from 0 degrees to 180 degrees
    for (pos = 0; pos <= 180; pos += 1)
    {
      // in steps of 1 degree
      myservo[i].write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    // goes from 180 degrees to 0 degrees
    for (pos = 180; pos >= 0; pos -= 1)
    {
      myservo[i].write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
  }
}

