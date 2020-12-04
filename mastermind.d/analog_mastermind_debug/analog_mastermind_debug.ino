#include <string.h>

#include "analog_mastermind_debug.h"

#define NUM_LED 7
#define ANS_NUM 4

// Debouncing stuff
int  buttonStateA; // current reading from the input pin
int  lastButtonStateA = LOW; // previous reading from the input pin
long lastDebounceTimeA = 0; // last time the output pin was toggled
int  buttonStateB; // current reading from the input pin
int  lastButtonStateB = LOW; // previous reading from the input pin
long lastDebounceTimeB = 0; // last time the output pin was toggled
int  buttonStateC; // current reading from the input pin
int  lastButtonStateC = LOW; // previous reading from the input pin
long lastDebounceTimeC = 0; // last time the output pin was toggled

long debounceDelay = 50; // debounce time; increase if the output flickers
// this will be randomly generated
int passcode[ANS_NUM];
// users input created by testPin
int answer[ANS_NUM];
// leds array
int leds[NUM_LED];
// result (which colors are good)
int result[ANS_NUM];
// what colors were picked; initially all set to 0
int used[NUM_LED];

/*
 * Used by calibrate, re_calibrate, and testPin
 */
int raw = 0;      // variable to store the raw input value
int Vin = 5;      // variable to store the input voltage
float Vout = 0;   // variable to store the output voltage
float R1 = 10;    // variable to store the R1 value
float R2 = 0;     // variable to store the R2 value
float buffer = 0; // buffer variable for calculation

float highest = 0;
float lowest = 1000;

float highs[] = { 
  -1,   -1,   -1,   -1,   -1,   -1,   -1 };   // high values
float lows[] =  { 
  1000, 1000, 1000, 1000, 1000, 1000, 1000 }; // low

int dataDigit;
int dataResult;

// for shift register leds:
byte data4[] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

// For non-delay
long winPreviousMillis = 0;
long losePreviousMillis = 0;
long blipPreviousMillis = 0;

const long interval = 300;

// Flags
int resetFlag = false;
int firstFlag = true;
int winFlag = false;
int loseFlag = false;
int winSwapFlag = false;
int loseSwapFlag = false;
int shuffleFlag = true;

int loopFlag = true;
int tmpFlag = true;
int viewResultFlag = false;

// counters
int correct_index = 0;
int correct_counter = 0;
int num_tries = 0;

const int max_tries = 10;

void setup()
{
  //  int test_reading;
  int readingA;
  int readingB;
  int stopFlag = false;
  int cur_led = LED_0;

  randomSeed(analogRead(A0));
  //  Serial.begin(9600);
  for (int i = 0; i < ANS_NUM; i++)
    answer[i] = result[i] = LED_OFF;
  for (int i = 0; i < NUM_LED; i++)
  {
    used[i] = 0;
    leds[i] = cur_led++;
  }
  //  pinMode(testButtonPin, INPUT_PULLUP);
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinC, INPUT);
  pinMode(alertPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(latchPinA, OUTPUT);
  //  shiftClear(S_BOTH);
  //  delay(100);
  //  create_passcode();
  printDigit4(0, 0, true);
  printDigit4(1, 1, false);
  printDigit4(2, 2, false);
  printDigit4(3, 3, false);
  shiftClear(S_DIGIT);
  delay(500);
  blip(3, 100);
}

boolean hFlag = true;
boolean newFlag = false;
boolean useVolt = true;
boolean newVolt = false;
boolean firstVolt = true;

void loop()
{
  int readingB;
  int readingC;
  int readingA;
  answer[3] = testPin(3);
  if (answer[3] != LED_OFF)
  {
    readingB = digitalRead(buttonPinB);
    if (readingB != lastButtonStateB)
      lastDebounceTimeB = millis();
    if ((millis() - lastDebounceTimeB) > debounceDelay)
    {
      if (readingB != buttonStateB)
      {
        buttonStateB = readingB;
        if (buttonStateB == LOW)
        {
          hFlag = !hFlag;
          newFlag = true;
        }
      }
    }
    lastButtonStateB = readingB;
    readingC = digitalRead(buttonPinC);
    if (readingC != lastButtonStateC)
      lastDebounceTimeC = millis();
    if ((millis() - lastDebounceTimeC) > debounceDelay)
    {
      if (readingC != buttonStateC)
      {  
        buttonStateC = readingC;
        if (buttonStateC == LOW)
        {
          highest = -1;
          lowest = 1000;
        }
      }
    }
    lastButtonStateC = readingC;
    readingA = digitalRead(buttonPinA);
    if (readingA != lastButtonStateA)
      lastDebounceTimeA = millis();
    if ((millis() - lastDebounceTimeA) > debounceDelay)
    {
      if (readingA != buttonStateA)
      {
        buttonStateA = readingA;
        if (buttonStateA == LOW)
        {
          printFloat(-1); // cheap way to clear the display
          newVolt = true;
          // swap volt with R2
          useVolt = !useVolt;
          firstVolt = true;
        }
      }
    }
    lastButtonStateA = readingA;
    if (firstVolt == true)
    {
      firstVolt = false;
      if (useVolt == true)
      {
        if (newVolt == true)
        {
          shiftClear(S_DIGIT);
          newVolt = false;
        }
        printMessage("V", 1000);
      }
      else 
      {
        if (newVolt == true)
        {
          shiftClear(S_DIGIT);
          newVolt = false;
        }
        printMessage("R", 1000);
      }
    }
    if (hFlag == true)
    {
      printMessage("H", 0);
      if (newFlag == true)
      {
        shiftClear(S_DIGIT);
        newFlag = false;
      }
      printFloat(highest);
    }
    if (hFlag == false)
    {
      printMessage("L", 0);
      if (newFlag == true)
      {
        shiftClear(S_DIGIT);
        newFlag = false;
      }
      printFloat(lowest);
    }
  }
#if 0
  else
  {
    digitalWrite(latchPinA, 0);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    digitalWrite(latchPinA, 1);
  }
#endif
  delay(500);
}

int testPin(int pin)
{
  int res = LED_OFF;

  raw = analogRead(pin);    // Reads the Input PIN
  Vout = (5.0 / 1023.0) * raw;    // Calculates the Voltage on th Input PIN
  buffer = (Vin / Vout) - 1;
  R2 = R1 / buffer;
  if (Vout < 5 && R2 < 100)
  {
    if (pin == 3)
    {
      if (highest < Vout)
        highest = (useVolt == true ? Vout : R2);
      if (lowest > Vout)
        lowest = (useVolt == true ? Vout : R2);
      //      printR2();
    }
    res = LED_G;
  }
  else
  {
    res = LED_OFF;
    for (int k = 0; k < 4; k++)
      printDigit4(k, 99, false);
  }
  return res;
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

void zero2nine()
{
  // loop through all printable digits for 7 segment LED
  for (int i = 0; i < 10; i++)
  {
    printDigit(i, false);
    delay(250);
  }
}

void blip(int times, int d)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(alertPin, HIGH);
    delay(d);
    digitalWrite(alertPin, LOW);
    delay(d);
  }
  /* attempt to not have to use delay
   for (int i = 0; i < times; i++)
   {
   unsigned long blipCurrentMillis = millis();
   digitalWrite(alertPin, HIGH);
   if (blipCurrentMillis - blipPreviousMillis > d)
   {
   blipPreviousMillis = blipCurrentMillis;
   digitalWrite(alertPin, LOW);
   }
   }
   */
}

// possibly combine both clears; and have a if flag
void shiftClear(int dr)
{
  byte d1 = 0b00000000;
  byte d2 = 0b00000000;

  digitalWrite(latchPin, 0);
  if (dr == S_RESULT)
    d1 = dataDigit;
  if (dr == S_DIGIT)
    d2 = dataResult;
  shiftOut(dataPin, clockPin, d1);
  shiftOut(dataPin, clockPin, d2);
  digitalWrite(latchPin, 1);
}

void printDigit(int num, int decimalFlag)
{
  byte Digit = (num > 9 ? 0b00000000 : digitArray[num]);

  if (decimalFlag == true)
    bitSet(Digit, 5);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, Digit);
  shiftOut(dataPin, clockPin, 0b00000000);
  digitalWrite(latchPin, 1);
}

void swap(int *a, int *b) { 
  int temp = *a; 
  *a = *b; 
  *b = temp; 
}

void printR2()
{
  float tmpT, tmpH;
  int tenth, hundth;

  printDigit4(0, R2 / 10, false);
  printDigit4(1, (int)R2 % 10, true);
  tmpT = R2 * 10;
  tenth = (int)tmpT % 10;
  printDigit4(2, tenth, false);
  tmpH = R2 * 100;
  hundth = (int)tmpH % 10;
  printDigit4(3, hundth, false);
}

void printDigit4(int pos, int num, boolean dp)
{
  int ctr = 0;
  byte myData;

  if (pos < 4 && (num > -1 && num < 10 || num == 99))
  {
    myData = (num == 99 ? 0b00000000 : digitArray4[num]);
    if (dp == true)
      bitSet(myData, 2);
    data4[pos] = myData;
    digitalWrite(latchPinA, 0);
    shiftOut(dataPinA, clockPinA, data4[ctr++]);
    shiftOut(dataPinA, clockPinA, data4[ctr++]);
    shiftOut(dataPinA, clockPinA, data4[ctr++]);
    shiftOut(dataPinA, clockPinA, data4[ctr++]);
    digitalWrite(latchPinA, 1);
  }
}

void printFloat(float num)
{
  float tmpT, tmpH;
  int tenth, hundth;

  if (num > 0 && num < 99)
  {
    printDigit4(0, num / 10, false);
    printDigit4(1, (int)num % 10, true);
    tmpT = num * 10;
    tenth = (int)tmpT % 10;
    printDigit4(2, tenth, false);
    tmpH = num * 100;
    hundth = (int)tmpH % 10;
    printDigit4(3, hundth, false);
  }
  else
  {
    digitalWrite(latchPinA, 0);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    shiftOut(dataPinA, clockPinA, 0b00000000);
    digitalWrite(latchPinA, 1);
  }
}

void printMessage(char *message, int d)
{
  for (int i = 0; i < strlen(message); i++)
  {
    byte b = (message[i] == 32 ? alphabetArray[26] : alphabetArray[message[i] - 65]);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, b);
    shiftOut(dataPin, clockPin, dataResult);
    digitalWrite(latchPin, 1);
    if (d != 0)
    {
      delay(d);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, 0b00000000);
      shiftOut(dataPin, clockPin, dataResult);
      digitalWrite(latchPin, 1);
    }
  }
}

