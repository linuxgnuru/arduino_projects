#include <SPI.h>
//#include <string.h>
//#include <ctype.h>


/* defines */
//#define INT_BAT_DEBUG
//#define SHOW_DEMO
#define USE_EXT_BAT
#define USE_SERVO
#define USE_ENCODER
#define USE_JOYPAD
#define USE_MATRIX

#ifdef USE_MATRIX
//#define USE_FONT
#ifdef USE_FONT
#include "font.h"
#endif
#endif
/* end defines */

#include "ghostbusters7.h"

/* debug stuff */
#define DEBUG
unsigned long dbLastTime;
/* end debug stuff */

/* 8x8 max something led matrix */
#ifdef USE_MATRIX
#include <LedControl.h>
boolean used[8][8];
unsigned long delaytime = 100;
unsigned long pM_0 = 0;
unsigned long pM_1 = 0; // for ledPrint
boolean sequenceDone_ledPrint = false;
boolean sequenceDone_ledPrintcd = false;
LedControl lc = LedControl(2, 3, 4, 1);
unsigned long matrix_lastM = 0;
#endif
/* end 8x8 max something led matrix */

/* Encoder */
#ifdef ENCODER
// Setting up the counter
int enc_reading = 0;
int enc_lastReading = -1;
int enc_lowest = 0;
int enc_highest = 17;
int enc_changeamnt = 1;
// Timing for polling the encoder
unsigned long enc_currentTime;
unsigned long enc_lastTime;
// Pin definitions
const byte enc_pinA = 8;
const byte enc_pinB = 9;
int enc_buttonPin;
int num_e = 0;
// Storing the readings
boolean enc_encA;
boolean enc_encB;
boolean enc_lastA = false;
boolean enc_encBool = false;
#endif
/* end encoder */

/* joypad */
#ifdef JOYPAD
// Debounce
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
#define MAX_WAIT 100
boolean jBool = false;
const byte joyPinY = A3;
const byte joyPinX = A2;
const byte joyButton = 7;
int x = 0;
int y = 0;
int jx, jy;
int num_j = 0;
int lastX = 500;
int lastY = 500;
#endif
/* end joypad */

/* 3 (now 2) lights */
#define MAX_LEDS 2
int lastLed = 1;
const byte ledY = 0;
const byte ledR = 1;
/* end 3 (now 2) lights */

/* grid */
int curGridLed = 9;
/* end grid */

/* NS shield */
#ifdef USE_EXT_BAT
#include <NS_energyShield.h>
#include <Wire.h>
// Global Variables
int Voltage, Current, AdapterVoltage;
float Percent, Temperature;
// Defines NS_energyShield object called "eS"
NS_energyShield eS;
#endif
/* end ns shield */

/* blinky lights */
boolean sequenceDone = false;
int blinkCtr = 0;
/* end blinky lights */

/* servo */
#ifdef SERVO
#include <Servo.h>
Servo lServo, rServo;  // create servo object to control a servo
#define MAX_SERVO 90
#define MIN_SERVO_L 180
#define MIN_SERVO_R 0
const byte lServoPin = 5;
const byte rServoPin = 6;
#endif
/* end servo */

/* analog sensor */
const byte analogPin = A1;
/* end analog sensor */

const byte srandPin = A0;

/* SPI */
const byte LATCH = 10;
/* end SPI */

void setup()
{
  randomSeed(analogRead(srandPin));
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  // BEGIN MATRIX
#ifdef USE_MATRIX
  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, 8);
  // and clear the display
  lc.clearDisplay(0);
#endif
  // END MATRIX
#ifdef ENCODER
  // BEGIN ENCODER - set the two pins as inputs with internal pullups
  pinMode(enc_pinA, INPUT_PULLUP);
  pinMode(enc_pinB, INPUT_PULLUP);
  // Set up the timing of the polling
  enc_currentTime = millis();
  enc_lastTime = enc_currentTime;
#endif
  // END ENCODER
  // Front LEDs for battery level
  pinMode(ledY, OUTPUT);
  pinMode(ledR, OUTPUT);
#ifdef DEBUG
  Serial.begin(9600);
#endif
#ifdef INT_BAT_DEBUG
  while (!Serial); // Wait for open COM port
  delay(5000);
#endif
#ifdef SERVO
  lServo.attach(lServoPin);
  rServo.attach(rServoPin);
  parkServo();
#endif
#ifdef SHOW_DEMO
  for (int i = 0; i < 18; i++)
  {
    printGrid(i);
    // since this is the start; who cares if we pause everything or not
    delay(75);
  }
  for (int mx = 0; mx < 8; mx++)
  {
    for (int my = 7; my > -1; my--)
    {
      GridMark(mx, my);
    }
  }
#endif
  curGridLed = num_j = getNum(x, y);
  printGrid(num_j);
  pinMode(joyButton, INPUT_PULLUP);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      used[i][j] = 0;
    }
  }
}

void loop()
{
  unsigned long curMillis = millis();
  int dl = 500;

#ifdef USE_MATRIX
#endif
#ifdef ENCODER
  printGrid(checkEncoder());
#endif
  getBatData();
  if (Percent <= 50)
  {
    digitalWrite(ledY, HIGH);
    digitalWrite(ledR, LOW);
  }
  else if (Percent <= 10)
  {
    digitalWrite(ledY, LOW);
    digitalWrite(ledR, HIGH);
  }
  else
  {
    digitalWrite(ledY, LOW);
    digitalWrite(ledR, LOW);
  }
  /*
    else
    {
    if (curMillis - lastMillis_led >= 750)
    {
      lastMillis_led = curMillis;
      randomLed();
    }
    }
  */
  if (curMillis - lastMillisA >= MAX_WAIT)
  {
    lastMillisA = curMillis;
    /*
                B
       +-----------------+
       |F      /|\      G|
       |        |        |
       |                 |
      C|<--     A     -->|D
       |                 |
       |                 |
       |        |        |
       |H      \|/      I|
       +-----------------+
                E

       A =  500,  500
       B = 1023,  500
       C =  500,    0
       D =  500, 1023
       E =  500, 1023
       F = 1023,    0
       G = 1023, 1023
       H =    0,    0
       I =    0, 1023
    */
    int buttonReading = digitalRead(joyButton);
    if (buttonReading != lastButtonState)
      lastDebounceTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      if (buttonReading != buttonState)
      {
        buttonState = buttonReading;
        if (buttonState == LOW)
          lc.clearDisplay(0);
      }
    }
    lastButtonState = buttonReading;
    x = map(analogRead(joyPinX), 1023, 0, 7, 0);
    y = map(analogRead(joyPinY), 0, 1023, 7, 0);
    GridMark(y, x);
#if 0
    jx = analogRead(joyPinX);
    jy = analogRead(joyPinY);
    // movement
    if ((jx >= 520 || jx <= 450) || (jy >= 520 || jy <= 450))
    {
      jBool = true;
      x = map(jx, 1023, 0, 0, 5);
      y = map(jy, 1023, 0, 0, 2);
      num_j = getNum(x, y);
      enc_lastReading = enc_reading;
      enc_reading = num_j;
      printGrid(num_j);
#ifdef DEBUG
      debugPrintAll();
#endif
    }
    else
    {
      if (enc_encBool == false && jBool == false)
      {
        num_j = 8;
        printGrid(num_j);
      }
    }
#endif
    dl = analogRead(analogPin);
#ifdef SERVO
    moveServo(dl);
#endif
    flickerLeds(dl);
    // let's get crazy!
    if (dl >= 800)
    {
      randomGrids();
    }
  }
}

int getNum(int x, int y)
{
  return (y + x + (y * 5));
}

#ifdef SERVO
void moveServo(int average)
{
  unsigned long curMillis = millis();

  int lVal = map(average, 0, 1023, MIN_SERVO_L, MAX_SERVO);
  int rVal = map(average, 0, 1023, MIN_SERVO_R, MAX_SERVO);
  if (curMillis - lastMillisB >= 50)
  {
    lastMillisB = curMillis;
    lServo.write(lVal);
    rServo.write(rVal);
  }
  /*
    else // park
    {
      parkServo();
    }
  */
}

void parkServo()
{
  lServo.write(MIN_SERVO_L);
  rServo.write(MIN_SERVO_R);
}
#endif

// num = 0 - 17
void printGrid(int num)
{
  if (num < 18 && num > -1)
  {
    curGridLed = num;
    //Serial.print(F(" [grid] num: ")); Serial.println(num);
    clearSPI();
    if (num == 0 || num == 1)
    {
      if (num == 0)
        bitSet(data[1], 0);
      if (num == 1)
        bitSet(data[0], 0);
    }
    else
    {
      //bitClear(data[0], 0);
      //bitClear(data[1], 0);
      if (num < 10)
        data[2] = grid[num];
      else
        data[3] = grid[num];
    }
    sendSPI();
  }
}

void clearSPI()
{
  bitClear(data[0], 7);
  bitClear(data[1], 7);
  data[2] = data[3] = 0;
  sendSPI();
}

void sendSPI()
{
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < 4; i++)
    SPI.transfer(data[i]); // ZZZZZ
  digitalWrite(LATCH, HIGH);
}

void flickerLeds(int num)
{
  unsigned long currentMillis = millis();
  unsigned long dl = map(num, 0, 1023, 500, 10);

  // The following is a for loop using millis instead of delay.
  if ((currentMillis - lastMillisBlink >= dl) && sequenceDone == false)
  {
    lastMillisBlink += dl;
    if (blinkCtr >= 6)
    {
      sequenceDone = true;
      blinkCtr = 0;
    }
    if (sequenceDone == false)
    {
      data[0] = data[1] = flashers[blinkCtr++];
      sendSPI();
    }
  }
  if (sequenceDone == true)
  {
    sequenceDone = false;
    blinkCtr = 0;
  }
}

void randomGrids()
{
  int ranA, ranB;
  boolean ok = false;
  static int lastM;
  static int lastL;
  static int lastXY;

  for (int i = 0; i < 10; i++)
  {
    while (ok == false)
    {
      ranA = random(0, 8);
      if (ranA != lastM)
        ok = true;
    }
    lastM = ranA;
    ok = false;
    while (ok == false)
    {
      ranB = random(0, 8);
      if (ranB != lastL)
        ok = true;
    }
    lastL = ranB;
    /* 
     * TODO: instead of using the following, just use two 
     *       bytes and use bitSet/bitRead
     */
    used[lastM][lastL] = 1;
    checkMax();
    GridMark(ranA, ranB);

    while (ok == false)
    {
      ranA = random(0, 17);
      if (ranA != lastXY)
        ok = true;
    }
    lastXY = ranA;
    printGrid(ranA);
  }
}

void randomLed()
{
  int ran;
  boolean ok = false;

  while (ok == false)
  {
    ran = random(0, MAX_LEDS);
    if (ran != lastLed)
      ok = true;
  }
  digitalWrite(ran, LOW);
  lastLed = ran;
  digitalWrite(ran, HIGH);
}

void getBatData()
{
  Voltage = eS.voltage(); // Voltage is returned in mV
  Current = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature() / 8; // Temperature is returned in 0.125 C increments
  AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage
#ifdef DEBUG
#ifdef INT_BAT_DEBUG
  // Print out results
  Serial.print(F("Voltage(mV) "));     Serial.print(Voltage);
  Serial.print(F(" Current(mA) "));    Serial.print(Current);
  Serial.print(F(" Charge "));         Serial.print(Percent, 1);
  Serial.print(F("% Temp "));          Serial.print(Temperature, 2);
  Serial.print(F("C\tAdapter V(mV)")); Serial.println(AdapterVoltage);
#endif
#endif
}

#ifdef ENCODER
int checkEncoder()
{
  // Read elapsed time
  enc_currentTime = millis();
  // Check if it's time to read
  if (enc_currentTime >= (enc_lastTime + 5))
  {
    // read the two pins
    enc_encA = digitalRead(enc_pinA);
    enc_encB = digitalRead(enc_pinB);
    // check if A has gone from high to low
    if ((!enc_encA) && (enc_lastA))
    {
      enc_encBool = true;
      // check if B is high
      if (enc_encB)
      {
        // clockwise
        if (enc_reading + enc_changeamnt <= enc_highest)
        {
          enc_reading += enc_changeamnt;
        }
      }
      else
      {
        // anti-clockwise
        if (enc_reading - enc_changeamnt >= enc_lowest)
        {
          enc_reading -= enc_changeamnt;
        }
      }
    }
    // store reading of A and millis for next loop
    enc_lastA = enc_encA;
    enc_lastTime = enc_currentTime;
  }
  return enc_reading;
}
#endif

#ifdef USE_MATRIX
void GridMark(int x, int y)
{
  lc.setLed(0, y, x, true);
}

void GridClear(int x, int y)
{
  lc.setLed(0, y, x, false);
}
#endif

#ifdef DEBUG
void debugPrintAll()
{
  unsigned long currentMillis = millis();

  if (currentMillis - dbLastTime >= 1000)
  {
    dbLastTime = currentMillis;
    Serial.println(F("-- Useful Info --"));
    Serial.print(F(" (jx, jy): ("));      Serial.print(jx);  Serial.print(F(", ")); Serial.print(jy); Serial.print(F(")"));
    Serial.print(F(" --- (x, y): ("));    Serial.print(x);   Serial.print(F(", ")); Serial.print(y);  Serial.print(F(")"));
    Serial.print(F(" curGridLed: "));     Serial.print(curGridLed);
    if ((num_e >= 0) || (num_j >= 0))
    {
      if (num_e >= 0)
      {
        Serial.print(F(" num_e: [")); Serial.print(num_e); Serial.print(F("] "));
      }
      if (num_j >= 0)
      {
        Serial.print(F(" num_j: [")); Serial.print(num_j); Serial.print(F("] "));
      }
      Serial.println();
    }
    Serial.println(F("-- fin --"));
  }
}
#endif

void checkMax()
{
  int ctr = 0;
  if (used
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (used[i][j] == 1)
        ctr++;
    }
  }
  if (ctr >= 64)
  {
    Serial.println(F("ctr >= 64"));
    lc.clearDisplay(0);
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
        used[i][j] = 0;
  }
}

#if 0
void checkMax()
{
  int ctr = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (used[i][j] == 1)
        ctr++;
    }
  }
  if (ctr >= 64)
  {
    lc.clearDisplay(0);
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
        used[i][j] = 0;
  }
}
#endif
