int switchPin = 2;
int motor1Pin1 = 3;
int motor1Pin2 = 4;
int enablePin = 9;

void setup()
{
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, HIGH);
}

void loop()
{
  if (digitalRead(switchPin) == HIGH)
  {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  }
  else
  {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  }
}

