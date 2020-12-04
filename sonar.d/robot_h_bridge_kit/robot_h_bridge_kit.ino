/*  Ultrasonic Sensor HC-SR04 + h-bridge
        Trig: Trigger (OUTPUT)
        Echo: Echo (INPUT)*/
// h bridge pins
const int enPin1 = 11;
const int in1pin = 12;
const int in2pin = LED_BUILTIN;
const int enPin2 = 10;
const int in3pin =  9;
const int in4pin =  8;

// sonic pins (0 = left, 1 = center, 2 = right)
const int echoPins[] = { 2, 3, 4, A0 }; //int echoPin = 9;    // Echo - Orange Jumper
const int trigPins[] = { 5, 6, 7, A1 }; //int trigPin = 8;    // Trig - Yellow Jumper

const int threshHold = 3;
const int backTime = 2000;
unsigned long previousMillis = 0;

void setup()
{
  for (int i = 0; i < 4; i++)
    pinMode(echoPins[i], INPUT);
  for (int i = 5; i < 14; i++)
    pinMode(i, OUTPUT);
  pinMode(trigPins[3], OUTPUT);
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
  long duration;
  int cm, inches;
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPins[sensor], LOW);
  delayMicroseconds(5);
  digitalWrite(trigPins[sensor], HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPins[sensor], LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPins[sensor], INPUT);
  duration = pulseIn(echoPins[sensor], HIGH);
  // convert the time into a distance
  cm = duration / 58; //(duration/2) / 29.1;
  inches = (duration / 2) / 74; // 37
  return (inches < threshHold ? true : false);
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

