#include <Servo.h>

Servo countServo;
const int anaPin = A0;

int val = 0;
boolean isFirstLaunch = true;

void setup()
{

  Serial.begin(115200);

  //attach pin 7 to servo and sweep it to indicate that the timer is on
  countServo.attach(7);
  countServo.write(180);
  delay(700);
  countServo.write(0);
  delay(700);
  countServo.write(180);
}

void loop()
{
  //If the sensor reads something significantly different from what it's normally seeing (the empty sink)
  //we start the countdown!
  val = analogRead(anaPin);
  if (val > 600 && val < 644)
  {
      countdownServo();
  }
}

//this moves the servo 4.5 degrees every half second
//to smooth out the movemnt (vs 9 degrees every second)
//for 20 seconds.
void countdownServo()
{
  Serial.println("Counting down");
  int halfseconds = 40;

  for (int i = halfseconds; i >= 0; i--)
  {
    countServo.write((int)(i * 4.5));
    delay(500);
  }
  countServo.write(180);
  delay(700);
}
