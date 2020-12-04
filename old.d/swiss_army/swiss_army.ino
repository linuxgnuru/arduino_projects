#include "digit.h"
#include "mario.h"

const int tmpPin = A0;
const int ldrPin = A1;
const int potPin = A2;

const int dataPin  = A3;
const int latchPin = A4;
const int clockPin = A5;

const int butPin = 2;

const int ledBlockPins[] = { 1, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
const int rgbPins[] = { 9, 10, 11 }; // rgbPins[RPIN]
const int buzzPin = LED_BUILTIN;
const int tiltPin = 12;
//const int ledPin = 0;

int  tiltState; // current reading from the input pin
int  lastTiltState = LOW; // previous reading from the input pin
long lastTiltDebounceTime = 0; // last time the output pin was toggled
int  butState; // current reading from the input pin
int  lastButState = LOW; // previous reading from the input pin
long lastButDebounceTime = 0; // last time the output pin was toggled
long debounceDelay = 50; // debounce time; increase if the output flickers

volatile int curFunction = 2;

void setup()
{
  pinMode(tiltPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
//  pinMode(ledPin, OUTPUT);
  pinMode(butPin, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(butPin), selectFunc, CHANGE);
  //  attachInterrupt(0, selectFunc, CHANGE);
  //  pinMode(butPin, INPUT_PULLUP);
  for (int i = 0; i < 10; i++)
    pinMode(ledBlockPins[i], OUTPUT);
  Serial.begin(9600);
//  printDigit(5, false);
//  delay(1000);
  mario();
}

void loop()
{
//  digitalWrite(1, HIGH);
  /*
  int p;
  long val;
  int reading;
  int readingB;

  readingB = digitalRead(butPin);
  if (readingB != lastButState)
    lastButDebounceTime = millis();
  if ((millis() - lastButDebounceTime) > debounceDelay)
  {
    if (readingB != butState)
    {
      butState = readingB;
      if (butState == LOW)
        curFunction = (curFunction + 1 > 5 ? 0 : curFunction + 1);
    }
  }
  lastButState = readingB;
  switch (curFunction)
  {
    case 0: case 1:
      p = (curFunction == 0 ? potPin : ldrPin);
      val = map(analogRead(p), 0, 1023, 0, 10);
      val = constrain(val, 0, 10);
      val = abs(val - 10);
      for (int i = 0; i < 10; i++)
        digitalWrite(ledBlockPins[i], LOW);
      for (int i = 0; i < val; i++)
        digitalWrite(ledBlockPins[i], HIGH);
      break;
    case 2:
      for (int j = 0; j < 9; j++)
      {
        printDigit(j, false);
        delay(250);
      }
      //getTemp();
      break;
    case 3:
      colorCycle();
      break;
    case 4: // tilt
      reading = digitalRead(tiltPin);
      if (reading != lastTiltState)
        lastTiltDebounceTime = millis();
      if ((millis() - lastTiltDebounceTime) > debounceDelay)
      {
        if (reading != tiltState)
        {
          tiltState = reading;
          if (tiltState == HIGH)
            digitalWrite(ledPin, (digitalRead(ledPin) == HIGH ? LOW : HIGH));
        }
      }
      lastTiltState = reading;
      break;
    case 5:
      mario();
      break;
    default:
      break;
  }
  delay(100);

  */
}

void printDigit(int num, int decimalFlag)
{
  byte Digit = (num > 9 ? 0b00000000 : digitArray[num]);
  // print decimal
  //     543210
  // 0b00100000
  if (decimalFlag == true)
    bitSet(Digit, 7);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, Digit);
  digitalWrite(latchPin, 1);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int i = 0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    pinState = (myDataOut & (1 << i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

void colorCycle()
{
  unsigned int rgb[3];
  int rev_dec;

  rgb[0] = 255;
  rgb[1] = rgb[2] = 0;
  for (int decrement = 0; decrement < 3; decrement++)
  {
    rev_dec = abs(decrement - 2);
    int increment = decrement == 2 ? 0 : decrement + 1;
    for (int i = 0; i < 255; i += 1)
    {
      rgb[decrement] -= 1;
      rgb[increment] += 1;
      analogWrite(rgbPins[RPIN], rgb[0]);
      analogWrite(rgbPins[BPIN], rgb[1]);
      analogWrite(rgbPins[GPIN], rgb[2]);
      /*
            if (curFunction != 2)
              break;
            else
      */
      delay(5);
    }
    //    if (curFunction != 2)      break;
  }
}

int high = -1;
int low = 1000;
void mario()
{
  int thisNote;
  int noteDuration;
  int pauseBetweenNotes;
  int size;
  long v;

  for (thisNote = 0; thisNote < 98; thisNote++)
  {
    noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzPin, melody[thisNote], noteDuration);
    pauseBetweenNotes = noteDuration * 1.30;
//#if 0
//    Serial.print("h= "); Serial.print(high); Serial.print(" l= "); Serial.println(low);
    v = map(melody[thisNote], 165, 523, 0, 10);
    v = constrain(v, 0, 10);
    //v = abs(v - 10);
    for (int i = 0; i < 10; i++)      digitalWrite(ledBlockPins[i], LOW);
    for (int i = 0; i < v; i++)      digitalWrite(ledBlockPins[i], HIGH);
//#endif
    delay(pauseBetweenNotes);
    noTone(buzzPin);
#if 0
    if (curFunction != 4)
    {
      noTone(buzzPin);
      break;
    }
    else
    {
      delay(pauseBetweenNotes);
      noTone(buzzPin);
    }
#endif
  }
}

void getTemp()
{
  float voltage, degreesC, degreesF;
  float tmpT, tmpH;
  int tenth, hundth;
  int d = 500;

  voltage = getVoltage(tmpPin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0 / 5.0) + 32.0;
  printDigit(degreesF / 10, false);
  delay(d);
  printDigit((int)degreesF % 10, true);
  delay(d);
  tmpT = degreesF * 10;
  tenth = (int)tmpT % 10;
  printDigit(tenth, false);
  delay(d);
  tmpH = degreesF * 100;
  hundth = (int)tmpH % 10;
  printDigit(hundth, false);
  delay(d);
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}

void selectFunc()
{
  curFunction = (curFunction + 1 > 5 ? 0 : curFunction + 1);
}
