int enablePin = 5;
int in1Pin = 3;
int in2Pin = 4;
int switchPin = 8;
//int potPin = 0;
 
void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(switchPin, INPUT);
}
 
void loop()
{
  int speed = 160;//analogRead(potPin) / 4;
  boolean reverse = !digitalRead(switchPin);
  setMotor(speed, reverse);
}
 
void setMotor(int speed, boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, reverse);
  digitalWrite(in2Pin, !reverse);
}
