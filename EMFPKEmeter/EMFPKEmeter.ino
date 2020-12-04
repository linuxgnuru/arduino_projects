//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __|
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/
//
// EMF - PKE meter
//
// Made by John Wiggins
// License: GPL 3.0
// Downloaded from: https://circuits.io/circuits/3142539-the-unnamed-circuit

#include <SPI.h>
#include <Servo.h>

Servo servoL;  // create servo object to control a servo
Servo servoR;  // create servo object to control a servo

const int LATCH = 10;
const int servoPinL = 6;
const int servoPinR = 7;
/*
  data[0] 7-2 = left leds, data[0] 0 == grid
  e.g.
  bitWrite(data[0], 0, 1);
*/
byte data[4];

unsigned long lastMillis = 0;

/*
   for EMF
*/
#define NUMREADINGS 15 // raise this number to increase data smoothing

int senseLimit = 15; // raise this number to decrease sensitivity (up to 1023 max)
int probePin = 5; // analog 5
int val = 0; // reading from probePin

// variables for smoothing
int readings[NUMREADINGS];                // the readings from the analog input
int index = 0;                            // the index of the current reading
int total = 0;                            // the running total
int average = 0;                          // final average of the probe reading

//CHANGE THIS TO affect the speed of the updates for numbers. Lower the number the faster it updates.
int updateTime = 40;

void setup()
{
  for (int i = 0; i < NUMREADINGS; i++)
    readings[i] = 0;
  servoL.attach(servoPinL);  // attaches the servo on pin 9 to the servo object
  servoR.attach(servoPinR);  // attaches the servo on pin 9 to the servo object
  pinMode(LATCH, OUTPUT);
  SPI.begin();
}

void loop()
{
  unsigned long curMillis;
  curMillis = millis();
  if (curMillis - lastMillis >= updateTime)
  {
    lastMillis = curMillis;
    data[2] = 0b00000000;
    data[3] = 0b00000000;
    val = analogRead(probePin);
    if (val >= 1)
    {
      val = constrain(val, 1, senseLimit);
      val = map(val, 1, senseLimit, 1, 1023);
      total -= readings[index];
      readings[index] = val;
      total += readings[index];
      index++;
      if (index >= NUMREADINGS)
        index = 0;
      average = total / NUMREADINGS;
      flashLed(average);
      /*
        if (average >  50)showLED0();
        if (average > 150)showLED1();
        if (average > 250)showLED2();
        if (average > 350)showLED3();
        if (average > 450)showLED4();
        if (average > 550)showLED5();
        if (average > 650)showLED6();
        if (average > 750)showLED7();
        if (average > 850)showLED8();
        if (average > 950)showLED9();
      */
    }
  }
}

void flashLed(int v)
{
  int m = map(v, 50, 950, 0, 6);
  switch (m)
  {
    case 0: data[0] = data[1] = 0b00000001; break;
    case 1: data[0] = data[1] = 0b00000011; break;
    case 2: data[0] = data[1] = 0b00000111; break;
    case 3: data[0] = data[1] = 0b00001111; break;
    case 4: data[0] = data[1] = 0b00011111; break;
    case 5: data[0] = data[1] = 0b00011111; break;
    case 6: data[0] = data[1] = 0b00111111; break;
  }
  digitalWrite(LATCH, LOW);
  SPI.transfer(data[3]);
  SPI.transfer(data[2]);
  SPI.transfer(data[1]);
  SPI.transfer(data[0]);
  digitalWrite(LATCH, HIGH);

}

