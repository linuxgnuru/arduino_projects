/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int val;    // variable to read the value from the analog pin

void setup()
{
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

int maxVal = 1023;

int maxPin = 0;

int degreesRot[] = {
  179,
  149,
  119,
  59,
  29,
  0
};

void loop()
{
  #if 1
  int num = 0;
  for (int aPin = A0; aPin < A5 + 1; aPin++)
  {
    val = analogRead(aPin);
    if (val < maxVal)
    {
      maxVal = val;
      maxPin = aPin;
    } 
  }
  switch (maxPin)
  {
    case A0:
     val = degreesRot[0];
      break;
    case A1:
     val = degreesRot[1];
      break;
    case A2:
     val = degreesRot[2];
      break;
    case A3:
     val = degreesRot[3];
      break;
    case A4:
     val = degreesRot[4];
      break;
    case A5:
     val = degreesRot[5];
      break;
  }
//  val = map(val, 0, 1023, 0, 180);
Serial.print("val: ");
  Serial.println(val);
  myservo.write(val);
  delay(15);
#else
  val = analogRead(A5);
    Serial.print("val= ");
  Serial.println(val);
delay(250);

  #endif
}

