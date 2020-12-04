
int enablePin1 = 11;
int in1Pin = 10;
int in2Pin = 9;
int switchPin1 = 7;

int potPin1 = A0;

int enablePin2 = 12;
int in3Pin = 6;
int in4Pin = 5;
int switchPin2 = 8;

int potPin2 = A1;

int motorNum = 0;
 
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
}
 
void loop()
{
  int speed1 = analogRead(potPin1) / 4;
  boolean reverse1 = digitalRead(switchPin1);
  int speed2 = analogRead(potPin2) / 4;
  boolean reverse2 = digitalRead(switchPin2);
  analogWrite(enablePin1, speed1);
  digitalWrite(in1Pin, ! reverse1);
  digitalWrite(in2Pin, reverse1);
  analogWrite(enablePin2, speed2);
  digitalWrite(in3Pin, ! reverse2);
  digitalWrite(in4Pin, reverse2);

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
