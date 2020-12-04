#include <SPI.h>
#include <NS_energyShield.h>
#include <Wire.h>
#include <LedControl.h>

#include <string.h>
#include <ctype.h>

#include "font.h"

/* defines */
//#define INT_BAT_DEBUG
//#define SHOW_DEMO
#define SERVO
#define MATRIX_TIME
#define ENCODER
#define JOYPAD
/* end defines */

#include "ghostbusters5.h"
/*
const char messages[3][30] = {
  "Ghostbusters!!!   ",
  "Ghost near by!  ",
  "Nothing located."
};
*/

/* debug stuff */
//#define DEBUG
boolean FirstTime = true;
unsigned long dbLastTime;
/* end debug stuff */

/* 8x8 max something led matrix */
#ifdef MATRIX_TIME
#define MAX_MSGS 3
#define MAX_DELAY 250
#define MAX_LETTER 127
#define MAX_SZ 180
int lastMsgNum = 0;
// for ledPrint
int dl_ledPrint;
int dl_ledPrintcd;
int letterCtr_ledPrint = 0;
unsigned long prevMillis_ledPrint = 0;
unsigned long prevMillis_ledPrintcd = 0;
boolean sequenceDone_ledPrint = false;
boolean sequenceDone_ledPrintcd = false;
LedControl lc = LedControl(2, 3, 4, 1);
unsigned long delaytime = 500;
char msg[40];
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
#define MAX_WAIT 100
boolean jBool = false;
const byte joyPinY = A3;
const byte joyPinX = A2;
//const byte joyButton = 7;
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
const byte ledPins[] = { 0, 1 };
/* end 3 (now 2) lights */

/* grid */
int curGridLed = 9;
static int lastGridLed;
/* end grid */

/* NS shield */
// Global Variables
int Voltage, Current, AdapterVoltage;
float Percent, Temperature;
// Defines NS_energyShield object called "eS"
NS_energyShield eS;
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

/* SPI */
const byte LATCH = 10;
/* end SPI */

void setup()
{
  randomSeed(analogRead(A0));
  SPI.begin();
  // BEGIN MATRIX
#ifdef MATRIX_TIME
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
  for (int i = 0; i < MAX_LEDS; i++)
    pinMode(ledPins[i], OUTPUT);
  pinMode(LATCH, OUTPUT);
#ifdef DEBUG
  Serial.begin(9600);
#ifdef INT_BAT_DEBUG
  while (!Serial); // Wait for open COM port
  delay(5000);
#endif
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
    delay(75);
  }
#endif
  curGridLed = num_j = getNum(x, y);
  printGrid(num_j);
  //pinMode(joypadButton, INPUT_PULLUP);
}

void loop()
{
  unsigned long curMillis = millis();
  int dl = 500;

#ifdef MATRIX_TIME
  //if (curMillis - matrix_lastM >= 5000)  {    matrix_lastM = curMillis;
    // TODO: randomly create messages to show
    //makeMsg();
    ledPrint();
//  }
#endif
#ifdef ENCODER
  if (FirstTime == false)
    printGrid(checkEncoder());
#endif
  getBatData();
  if (Percent <= 50)
    toggleLED(0);
  else if (Percent <= 10)
    toggleLED(1);
  else
  {
    if (curMillis - lastMillis_led >= 750)
    {
      lastMillis_led = curMillis;
      randomLed();
    }
  }
  /*
    if (digitalRead(joyButton) == LOW)
    Serial.println(F("BUTTON"));
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
    //if (enc_encBool == false)    {
    jx = analogRead(joyPinX);
    jy = analogRead(joyPinY);
    // movement
    if ((jx >= 520 || jx <= 450) || (jy >= 520 || jy <= 450))
    {
      FirstTime = false;
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
      //jBool = false;
    }
    //}
    dl = analogRead(analogPin);
#ifdef SERVO
    moveServo(dl);
#endif
    flickerLeds(dl);
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
    SPI.transfer(data[i]);
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
  digitalWrite(ledPins[lastLed], LOW);
  lastLed = ran;
  digitalWrite(ledPins[ran], HIGH);
}

// led = 0 - 1, y, r
void toggleLED(int led)
{
  for (int i = 0; i < MAX_LEDS; i++)
    digitalWrite(ledPins[i], (led == i ? HIGH : LOW));
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
    if ((!enc_encA) && (enc_lastA))// && jBool == false)
    {
      FirstTime = false;
      enc_encBool = true;
      /*
        if (reading != lastReading)
        reading = lastReading; */
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
    }/*
        else
        {
        jBool = false;
        enc_encBool = false;
        enc_lastReading = enc_reading;
        enc_reading = num_j;
        }*/
    // store reading of A and millis for next loop
    enc_lastA = enc_encA;
    enc_lastTime = enc_currentTime;
  }
  return enc_reading;
}
#endif

#ifdef MATRIX_TIME
void ledPrint()
{
  //#if 0
  unsigned long curMillis = millis();
  int ran;
  boolean ok = false;

  while (ok == false)
  {
    ran = random(0, 2);
    if (ran != lastMsgNum)
      ok = true;
  }
  lastMsgNum = ran;
  if (ran == 0)
    strcpy(msg, "Ghost near by!  ");
  else
    strcpy(msg, "Nothing located.");

  dl_ledPrint = 100;//delaytime - 250;
  dl_ledPrintcd = 500;
  if ((curMillis - prevMillis_ledPrint >= dl_ledPrint) && sequenceDone_ledPrint == false)
  {
    prevMillis_ledPrint += dl_ledPrint;
    if (letterCtr_ledPrint >= strlen(msg))
    {
      sequenceDone_ledPrint = true;
      letterCtr_ledPrint = 0;
    }
    if (sequenceDone_ledPrint == false)
    {
      if (sequenceDone_ledPrintcd == true)
      {
        for (int i = 0; i < 8; i++)
          lc.setColumn(0, i, font[msg[letterCtr_ledPrint]][i]);
        letterCtr_ledPrint++;
        sequenceDone_ledPrintcd = false;
      }
      if ((curMillis - prevMillis_ledPrintcd >= dl_ledPrintcd) && sequenceDone_ledPrintcd == false)
      {
        prevMillis_ledPrintcd += dl_ledPrintcd;
        lc.clearDisplay(0);
        sequenceDone_ledPrintcd = true;
      }
    }
  }
  if (sequenceDone_ledPrint == true)
  {
    sequenceDone_ledPrintcd = false;
    sequenceDone_ledPrint = false;
    letterCtr_ledPrint = 0;
  }
  //#endif
  #if 0
  for (int i = 0; i < strlen(msg); i++)
  {
    for (int j = 0; j < 8; j++)
      lc.setColumn(0, j, font[msg[i]][j]);
    delay(delaytime - 400);
    lc.clearDisplay(0);
    delay(delaytime - 400);
  }
#endif
}
/*
void makeMsg()
{
  int ran;
  boolean ok = false;

  while (ok == false)
  {
    ran = random(0, 2);
    if (ran != lastMsgNum)
      ok = true;
  }
  lastMsgNum = ran;
  if (ran == 0)
    strcpy(msg, "Ghost near by!  ");
  else
    strcpy(msg, "Nothing located.");
 ledPrint();
}
*/
#endif

