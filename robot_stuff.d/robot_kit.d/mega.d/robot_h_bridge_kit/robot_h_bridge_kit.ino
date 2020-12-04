/*  Ultrasonic Sensor HC-SR04 + h-bridge
        Trig: Trigger (OUTPUT)
        Echo: Echo (INPUT)*/
// h bridge pins
const int en1pin = 11;
const int in1pin = 12;
const int in2pin = LED_BUILTIN;

const int en2pin = 10;
const int in3pin =  9;
const int in4pin =  8;

const int threshHold = 3;
const int backTime = 2000;
unsigned long previousMillis = 0;

typedef enum {
  STOP,
  FORWARD,
  BACKUP
}
direction_enum;

void setup()
{
  for (int i = 5; i < 14; i++)
    pinMode(i, OUTPUT);
}

void loop()
{
  for (int i = 0; i < 3; i++)
  {
    if (testBlock(i))
    {
      switch (i)
      {
        case 0: // left of robot
          // left, right, delay, speed (0 - 255)
          // 0 = stop, 1 = forward, -1 = reverse
          runMotor(0, -1, backTime, 255);
          break;
        case 1: // center
          runMotor(-1, -1, backTime, 255);
          break;
        case 2: // right
          runMotor(-1, 0, backTime, 255);
          break;
        default:
          // never should get to this code.
          runMotor(0, 0, backTime, 255);
          break;
      }
    }
    else
    {
      runMotor(1, 1, 0, 255);
    }
  }
  //delay(250);
}

boolean testBlock(int sensor)
{
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
          analogWrite( en1pin, speed);
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
          analogWrite( en1pin, speed);
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
          analogWrite( en1pin, speed);
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
          analogWrite( en2pin, speed);
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
          analogWrite( en2pin, speed);
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
          analogWrite( en2pin, speed);
          digitalWrite(in3pin, HIGH);
          digitalWrite(in4pin, LOW);
        }
      }
      break;
  }
  //delay(dur);
}

