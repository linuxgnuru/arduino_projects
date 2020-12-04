const byte enablePin1 = 11;
const byte in1Pin = 10;
const byte in2Pin = 9;
const byte switchPin1 = 7;

const byte enablePin2 = 12;
const byte in3Pin = 6;
const byte in4Pin = 5;
const byte switchPin2 = 8;

//const byte potPin1 = A0;
//const byte potPin2 = A1;
const byte stopPin = 3;

int motorNum = 0;
boolean stopMe = false;

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
}

void loop()
{
  int speed1 = 130; // analogRead(potPin1) / 4;
  boolean reverse1 = digitalRead(switchPin1);
  int speed2 = 130; // analogRead(potPin2) / 4;
  boolean reverse2 = digitalRead(switchPin2);
  if (!digitalRead(stopPin))
  {
    analogWrite(enablePin1, speed1);
    digitalWrite(in1Pin, ! reverse1);
    digitalWrite(in2Pin, reverse1);
    analogWrite(enablePin2, speed2);
    digitalWrite(in3Pin, ! reverse2);
    digitalWrite(in4Pin, reverse2);
  }
  else
  {
    digitalWrite(enablePin1, LOW);
    digitalWrite(enablePin2, LOW);
  }
  //  setMotor(motorNum, speed1, reverse1);
}

/*
  void setMotor(int motorNum, int speed, boolean reverse)
  {
  int inA, inB, ePin;
  if (motorNum == 0)
  {
    inA = in1Pin;
    inB = in2Pin;
    ePin = enablePin1;
  }
  else
  {
    inA = in3Pin;
    inB = in4Pin;
    ePin = enablePin2;
  }
  analogWrite(eP, speed);
  digitalWrite(inA, ! reverse);
  digitalWrite(inB, reverse);
  }
*/

