// h bridge pins
const int enPin1 = 11;
const int in1pin = 12;
const int in2pin = LED_BUILTIN;
const int enPin2 = 10;
const int in3pin =  9;
const int in4pin =  8;

const int backTime = 2000;

boolean stopMe = false;
boolean turnMe = false;
int counter = 0;

void setup()
{
  for (int i = 8; i < 14; i++)
    pinMode(i, OUTPUT);
}

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
    counter++;
  }
  if (counter >= 3)
  {
    stopMe = true;
    runMotor(0, 0, 0, 0);
    counter = 0;
  }
}

void runMotor(int left, int right, int dur, int speed)
{
  unsigned long currentMillis = millis();
  switch (left)
  {
    case -1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite(enPin1, speed);
          digitalWrite(in1pin, LOW);
          digitalWrite(in2pin, HIGH);
        }
      }
      break;
    case 0:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite(enPin1, speed);
          digitalWrite(in1pin, LOW);
          digitalWrite(in2pin, LOW);
        }
      }
      break;
    case 1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite(enPin1, speed);
          digitalWrite(in1pin, HIGH);
          digitalWrite(in2pin, LOW);
        }
      }
      break;
  }
  switch (right)
  {
    case -1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite(enPin2, speed);
          digitalWrite(in3pin, LOW);
          digitalWrite(in4pin, HIGH);
        }
      }
      break;
    case 0:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite(enPin2, speed);
          digitalWrite(in3pin, LOW);
          digitalWrite(in4pin, LOW);
        }
      }
      break;
    case 1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite(enPin2, speed);
          digitalWrite(in3pin, HIGH);
          digitalWrite(in4pin, LOW);
        }
      }
      break;
  }
  //delay(dur);
}

