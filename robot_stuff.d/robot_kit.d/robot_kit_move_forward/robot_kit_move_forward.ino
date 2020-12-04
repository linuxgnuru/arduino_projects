// h bridge pins
const int enPin1 = 11; const int in1pin = 12; const int in2pin = LED_BUILTIN;
const int enPin2 = 10; const int in3pin =  9; const int in4pin =  8;

const int backTime = 2000;

void setup()
{
  for (int i = 8; i < 14; i++)
    pinMode(i, OUTPUT);
}

boolean stopMe = false, turnMe = false;
int fu = 0;

void loop()
{
  if (!stopMe && !turnMe)
  {
    runMotor(1, 1, 6000, 200);
    turnMe = true;
  }
  if (turnMe && !stopMe)
  {
    runMotor(-1, 1, 3000, 128);
    fu++;
  }
  if (fu >= 3)
  {
    stopMe = true;
    runMotor(0, 0, 0, 0);
  }
}

void runMotor(int left, int right, int dur, int speed)
{
  switch (left)
  {
    case 1:
      analogWrite(enPin1, speed);
      digitalWrite(in1pin, HIGH);
      digitalWrite(in2pin, LOW);
      break;
  }
  switch (right)
  {
    case 1:
      // save the last time you blinked the LED
      analogWrite(enPin2, speed);
      digitalWrite(in3pin, HIGH);
      digitalWrite(in4pin, LOW);
      break;
  }
  //delay(dur);
}

