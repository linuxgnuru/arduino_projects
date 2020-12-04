#include "rc_car_i2c_rpi.h"

#include <Wire.h>

#define SLAVE_ADDRESS 0x04

const int waitDelay = 1000;
unsigned long lastMillis = 0;
unsigned long lastMillisBreak = 0;

boolean runOnce = true;

int curDir = CENTER_;

// ranges for left and right:
//const float vFarLeftMax = 4.0;

volatile int command;
volatile int data;

const byte anaPin = A1;

const byte E1 = 5;
const byte I1 = 2;
const byte I2 = 3;

#define NUM_SONAR 4

#define TRIG 0
#define ECHO 1

const byte sonarPins[NUM_SONAR][2] = {
// output  input
// trigger echo
// white   brown
  {  9,  8 }, // left sensor
  { A2, A3 }, // center sensor
  { A0, 12 }, // right sensor
  { 11, 10 }  // rear sensor
};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // because we are only using half of the h bridge; ground the other half
  pinMode(4, OUTPUT); digitalWrite(4, LOW);
  pinMode(6, OUTPUT); digitalWrite(6, LOW);
  pinMode(7, OUTPUT); digitalWrite(7, LOW);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  for (uint8_t i = 0; i < NUM_SONAR; i++)
  {
    pinMode(sonarPins[i][TRIG], OUTPUT);
    pinMode(sonarPins[i][ECHO], INPUT);
  }
}

void loop()
{
  unsigned long currentMillis = millis();

  if (runOnce)
  {
    if (getDir() != CENTER_)
      setCenter();
    runOnce = false;
  }
  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
  }
}

void receiveData(int byteCount)
{
  while (Wire.available())
  {
    command = Wire.read();
    switch (command)
    {
      case CHECK_SONAR:
        boolean f;
        f = true;
        for (int i = CHECK_SONAR_LEFT; i <= CHECK_SONAR_REAR; i++)
        {
          int r = checkDistance(i);
          if (r == i + 8)
          {
            data = r;
            f = false;
            break;
          }
        }
        if (f)
          data = OK_;
        break;
      case CHECK_SONAR_LEFT: case CHECK_SONAR_CENTER: case CHECK_SONAR_RIGHT: case CHECK_SONAR_REAR:
        data = checkDistance(command);
        break;
      case GO_RIGHT:
        data = runServo(RIGHT_);
        break;
      case GO_LEFT:
        data = runServo(LEFT_);
        break;
      case GO_CENTER:
        setCenter();
        data = getDir();
        break;
      case GET_DIR:
        data = getDir();
        break;
      default:
        data = BAD_;
        break;
    }
  }
}

void sendData()
{
  Wire.write(data);
}

float getData()
{
  int raw;
  float R2 = 0;
  int c = 0;

  // Get an average
  for (int i = 0; i < 10; i++)
  {
    raw = analogRead(anaPin);
    //if (raw < 1020)
    float Vout = (5.0 / 1023.0) * raw;
    if (Vout != 5.0)
    {
      R2 += (10 / ((5 / Vout - 1)));
      c++;
    }
  }
  if (c != 0)
    R2 /= c;
  else
    R2 = 2.5;
  return R2;
}

void setCenter()
{
  int cd = getDir();
  float r2 = getData();
  unsigned long cM;

  while (cd != CENTER_)
  {
    // try to break out if it's running too long
    cM = millis();
    if (cM - lastMillisBreak >= 5000)
    {
      lastMillisBreak = cM;
      cd = CENTER_;
    }
    else
    {
      runServo(CENTER_);
      cd = getDir();
    }
  }
}

int getDir()
{
  int r = CENTER_;
  float rData = getData();

  if (rData > 3.0) // right
    r = (rData >= 3.4 ? FAR_RIGHT_ : RIGHT_);
  else if (rData >= 2.4 && rData <= 2.9) // middle
    r = CENTER_;
  else if (rData < 2.4) // left
    r = (rData <= 1.4 ? FAR_LEFT_ : LEFT_);
  return r;
}

int runServo(int dir)
{
  int ret_ = OK_;
  int c;

  switch (dir)
  {
    case RIGHT_: case FAR_RIGHT_:
      c = getDir();
      if (c == FAR_RIGHT_)
        ret_ = c;
      else
      {
        if (c == CENTER_)
        {
          while (getDir() != FAR_RIGHT_)
          {
            digitalWrite(I2, LOW);
            digitalWrite(I1, HIGH);
            analogWrite(E1, 255);
          }
        }
        if (c == LEFT_ || c == FAR_LEFT_) // left
        {
          while (getDir() != CENTER_)
          {
            digitalWrite(I2, LOW);
            digitalWrite(I1, HIGH);
            analogWrite(E1, 255);
          }
        }
        ret_ = getDir();
      }
      break;
    case CENTER_:
      c = getDir();
      if (c == CENTER_)
        ret_ = c;
      else
      {
        if (c == LEFT_ || c == FAR_LEFT_) // left
        {
          while (getDir() != CENTER_)
          {
            digitalWrite(I2, LOW);
            digitalWrite(I1, HIGH);
            analogWrite(E1, 255);
          }
        }
        if (c == RIGHT_ || c == FAR_RIGHT_) // right
        {
          while (getDir() != CENTER_)
          {
            digitalWrite(I2, HIGH);
            digitalWrite(I1, LOW);
            analogWrite(E1, 255);
          }
        }
        ret_ = getDir();
      }
      break;
    case LEFT_: case FAR_LEFT_:
      c = getDir();
      if (c == FAR_LEFT_)
        ret_ = c;
      else
      {
        if (c == CENTER_)
        {
          while (getDir() != FAR_LEFT_)
          {
            digitalWrite(I2, HIGH);
            digitalWrite(I1, LOW);
            analogWrite(E1, 255);
          }
        }
        if (c == RIGHT_ || c == FAR_RIGHT_) // right
        {
          while (getDir() != CENTER_)
          {
            digitalWrite(I2, HIGH);
            digitalWrite(I1, LOW);
            analogWrite(E1, 255);
          }
        }
        ret_ = getDir();
      }
      break;
    default:
      ret_ = BAD_;
      break;
  }
  digitalWrite(I1, LOW);
  digitalWrite(I2, LOW);
  analogWrite(E1, 0);
  return ret_;
}

int checkDistance(int sonarNum)
{
  long duration;
  double cm;
  //int inches;
  int threshHold = 13;
  int ret = OK_;
  int sonarPin = sonarNum - 3;

  if (sonarPin < 0 || sonarPin > NUM_SONAR)
    return BAD_;
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(sonarPins[sonarPin][TRIG], LOW);
  delayMicroseconds(5);
  digitalWrite(sonarPins[sonarPin][TRIG], HIGH);
  delayMicroseconds(10);
  digitalWrite(sonarPins[sonarPin][TRIG], LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(sonarPins[sonarPin][ECHO], INPUT);
  duration = pulseIn(sonarPins[sonarPin][ECHO], HIGH);
  // convert the time into a distance
  cm = duration / 58; //(duration/2) / 29.1;
  //inches = (duration / 2) / 74; // 37
  if (cm < threshHold)
    ret = sonarNum + 8;
  return ret;
}

