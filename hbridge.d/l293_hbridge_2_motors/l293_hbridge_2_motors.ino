/*
Adafruit Arduino - Lesson 15. Bi-directional Motor
*/

#define LEFT 1
#define RIGHT 2

int enablePin1 = 11;
int in1Pin = 10;
int in2Pin = 9;

int enablePin2 = 6;
int in3Pin = 4;
int in4Pin = 8;

int switchPin = 7;
int potPin = A0;

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
}

void loop()
{
  int speed = analogRead(potPin) / 4;
  boolean reverse = digitalRead(switchPin);
  setMotor(speed, reverse, LEFT);
}

void setMotor(int speed, boolean reverse, int motor_num)
{
  switch (motor_num)
  {
    case LEFT:
      analogWrite(enablePin1, speed);
      digitalWrite(in1Pin, ! reverse);
      digitalWrite(in2Pin, reverse);
      break;
    case RIGHT:
      analogWrite(enablePin2, speed);
      digitalWrite(in3Pin, ! reverse);
      digitalWrite(in4Pin, reverse);
      break;
  }
}

