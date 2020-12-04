#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 

const int leftPin = 7;
const int rightPin = 8;
const int servoPin = 9;

int left = LOW;
int right = LOW;
int val = 89;

void setup()
{
  pinMode(rightPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  myservo.attach(servoPin);
}

void loop()
{
  left = digitalRead(leftPin);
  right = digitalRead(rightPin);
  if (left == HIGH && right == HIGH)
    val = 89;
  else if (left == HIGH && right == LOW)
    val = 179;
  else if (left == LOW && right == HIGH)
    val = 0;
  else if (left == LOW && right == LOW)
    val = 89;
  myservo.write(val);
  delay(50);
}
