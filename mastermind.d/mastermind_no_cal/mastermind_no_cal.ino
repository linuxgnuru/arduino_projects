/*
  Mastermind using LEDs
 */
#include <string.h>

#include "no_cal.h"

#define NUM_LED 6
#define ANS_NUM 4
#define DEBUG 1

typedef enum {
  S_DIGIT,
  S_RESULT,
  S_BOTH
} clear_enum;

typedef enum {
  LED_G,
  LED_R,
  LED_ROFF
} status_enum;

typedef enum {
  LED_SR, // square red
  LED_BR,
  LED_BY,
  LED_BG,
  LED_BB,
  LED_LW, // little white
  LED_UOFF
} led_enum;

const float colorCode[][2] = {
  { 1.67, 1.71 }, // rec red
  { 1.74, 1.75 }, // big red
  { 1.81, 1.82 }, // big yellow
  { 2.27, 2.30 }, // big green
  { 2.55, 2.56 }, // big blue
  { 2.59, 2.65 }  // little white
};

int alertPin = LED_BUILTIN;

int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

int buttonPinA = 5;
int buttonPinB = 6;
int buttonPinC = 7;

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

int raw = 0;      // variable to store the raw input value
int Vin = 5;      // variable to store the input voltage
float Vout = 0;   // variable to store the output voltage
float R1 = 10;    // variable to store the R1 value
float R2 = 0;     // variable to store the R2 value
float buffer = 0; // buffer variable for calculation

int dataDigit;
int dataResult;

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

int tmpFlag = true;
int viewResultFlag = false;

// counters
int correct_index = 0;
int correct_counter = 0;
int num_tries = 0;

const int max_tries = 10;

void setup()
{
  //int test_reading;
  int readingA;
  int readingB;
  int stopFlag = false;
  int cur_led = LED_BB;
  randomSeed(analogRead(A5));
Serial.begin(9600);
  for (int i = 0; i < ANS_NUM; i++)
    answer[i] = result[i] = LED_ROFF;
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

//  shiftClear(S_BOTH);
  delay(100);
  create_passcode();
  //  blip(3, 100);
  //  blinkAll_2Bytes(4, 250);
  //  zero2nine();
  //  green2red(400);
//  shiftClear(S_RESULT);
  printMessage("GO", 400);
//  printMessage("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 400);
  /*
  // Pause giving time to remove the LEDs
  while (stopFlag == false)
  {
   readingA = digitalRead(buttonPinA);
   if (readingA != lastButtonStateA)
   lastDebounceTimeA = millis();
   if ((millis() - lastDebounceTimeA) > debounceDelay)
   {
     if (readingA != buttonStateA)
     {
       buttonStateA = readingA;
       if (buttonStateA == LOW)
         stopFlag = true;
     }
   }
   lastButtonStateA = readingA;
   delay(10);
 }
   */
//  blip(3, 100);
  shiftClear(S_DIGIT); // S_DIGIT, S_RESULT, or S_BOTH
}

void loop()
{
  /*
  byte fu = 0b00000000;
  for (int i = 0; i < 8; i++)
  {
    bitSet(fu, i);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, fu);
    shiftOut(dataPin, clockPin, 0b00000000);
    digitalWrite(latchPin, 1);
    delay(500);
    bitClear(fu, i);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, fu);
    shiftOut(dataPin, clockPin, 0b00000000);
    digitalWrite(latchPin, 1);
    delay(500);
  }
 fu = 0b00000000;
 for (int i = 0; i < 8; i++)
  {
    bitSet(fu, i);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0b00000000);
    shiftOut(dataPin, clockPin, fu);
    digitalWrite(latchPin, 1);
    delay(500);
    bitClear(fu, i);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, fu);
    shiftOut(dataPin, clockPin, 0b00000000);
    digitalWrite(latchPin, 1);
    delay(500);
  }
*/
  int readingA;
  int readingB;
  int readingC;
  int breakWhile = false; // used to wait until all LEDs are put in

  if (firstFlag == true)
    firstFlag = false;
  /*
    Steps:
   1) for loop; read in each analog
   2) if all each one has Vout < 5, then all have been inserted
   3)
   */
  // pause
  while (breakWhile == false)
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
          printPass();
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
          printNumLeft();
          //zero2nine();
//          shiftClear(S_DIGIT);
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
          blip(4, 100);
          for (int i = 0; i < ANS_NUM; i++)
            answer[i] = testPin(i);
          Serial.print("ans: ");
          for (int i = 0; i < ANS_NUM; i++)
          {
            Serial.print(answer[i]);
            Serial.print(" ");
          }
          Serial.println();
          for (int i = 0; i < ANS_NUM; i++)
          {
            printColor(answer[i]);
            Serial.print(" ");
          }
          Serial.println();
//          delay(100);
/*

NEW CODE
    if (winFlag == false && (key != '*' && key != '#'))
      answer[correct_index++] = key;

*/
#if 0
      correct_index = 0;
      // test for a correct code
      for (int i = 0; i < ANS_NUM; i++)
      {
        if (passcode[i] == answer[i])
        {
          correct_counter++;
          result[i] = LED_G;
        }
      }
      if (correct_counter >= ANS_NUM)
        winFlag = true;
      else
      {
        correct_counter = 0;
        for (int i = 0; i < ANS_NUM; i++)
        {
          if (used[i] == 1)
            result[i] = LED_G;
          if (result[i] == LED_ROFF)
            result[i] = LED_R;
        }
        num_tries++;
      }
      // Test to see if you lost
      if (num_tries > max_tries)
      {
        loseFlag = true;
        winFlag = false;
      }
    }
#endif // end if 0
/*

END NEW CODE


*/
          if (answer[0] != LED_UOFF && answer[1] != LED_UOFF && answer[2] != LED_UOFF && answer[3] != LED_UOFF)
            breakWhile = true;
        }
      }
    }
    lastButtonStateA = readingA;
  }
  int correct = 0;
  for (int i = 0; i < ANS_NUM; i++)
  {
    if (answer[i] == leds[passcode[i]])
      result[i] = 1;
  }
  if (result[0] == 1 && result[1] == 1 && result[2] == 1 && result[3] == 1)
  {
    winFlag = true;
    WinFunc();
  }
  if (loseFlag == true)
    LoseFunc();
  if (winFlag == false && loseFlag == false)
  {
#if 0
    if (result[NUM_LEDS-1] != LED_ROFF)
    {
      // Shuffle the result
      if (shuffleFlag == true)
      {
        for (int i = ANS_NUM - 1; i > 0; i--)
          swap(&result[i], &result[random(0, i)]);
        for (int i = 0; i < ANS_NUM; i++)
          changeBit(i, result[i]);
        shuffleFlag = false;
      }
    }
#endif
    printNumLeft();  // Insert printRemainingTries here.
  }
  // TODO: insert code to reset everything here
  delay(500);
}

int testPin(int pin)
{
  int res = LED_UOFF;

  raw = analogRead(pin);    // Reads the Input PIN
  Vout = (5.0 / 1023.0) * raw;    // Calculates the Voltage on th Input PIN
  buffer = (Vin / Vout) - 1;
  R2 = R1 / buffer;
  if (Vout < 5 && R2 < 100)
  {
    if (Vout >= 1.57 && Vout <= 1.71) res = LED_SR;
    else if (Vout >= 1.74 && Vout <= 1.75) res = LED_BR;
    else if (Vout >= 2.27 && Vout <= 2.30) res = LED_BG;
    else if (Vout >= 1.81 && Vout <= 1.82) res = LED_BY;
    else if (Vout >= 2.55 && Vout <= 2.56) res = LED_BB;
    else if (Vout >= 2.59 && Vout <= 2.65) res = LED_LW;
    Serial.print("Pin: ");
    Serial.print(pin);
    Serial.print(" Vout: ");
    Serial.println(Vout);
  }
  else
    res = LED_UOFF;
  printColor(res);
  Serial.println();
  return res;
}

void changeBit(int pos, int color)
{
  // pos = 0 - 3
  // color = LED_G, LED_R, or LED_ROFF
  /*
   3   2   1   0
   r 7 r 5 r 3 r 1
   g 6 g 4 g 2 g 0
   */
  switch (color)
  {
  case LED_G:
    bitSet(dataResult, (pos * 2));
    bitClear(dataResult, (pos * 2) + 1);
    break;
  case LED_R:
    bitClear(dataResult, (pos * 2));
    bitSet(dataResult, (pos * 2) + 1);
    break;
  default: 
  case LED_ROFF:
    bitClear(dataResult, (pos * 2));
    bitClear(dataResult, (pos * 2) + 1);
    break;
  }
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, dataDigit);
  shiftOut(dataPin, clockPin, dataResult);
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

void blinkAll_2Bytes(int n, int d)
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
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

void green2red(int del)
{
  int d1 = 0b00000000;
  int d2 = 0b00000000;
  int Lights[] = { 
    LED_G, LED_R   };

  dataResult = 0b00000000;
  for (int i = 3; i > -1; i--)
  {
    for (int j = 0; j < 2; j++)
    {
      changeBit(i, Lights[j]);
      delay(del);
    }
  }
  shiftClear(S_RESULT);
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

void create_passcode()
{
  for (int i = 0; i < NUM_LED; i++)
    used[i] = 0;
  for (int cur_index = 0; cur_index < ANS_NUM; cur_index++)
  {
    int ran = random(0, NUM_LED - 1);
    used[ran] = 1;
    passcode[cur_index] = ran;
  }
#ifdef DEBUG
  Serial.println("");
  Serial.print("[Pass: ");
  for (int i = 0; i < ANS_NUM; i++)
  {
    Serial.print(passcode[i]);
    Serial.print(" ");
  }
  Serial.println("]");
  decodePass();
#endif
}

void printPass()
{
  for (int i = 0; i < ANS_NUM; i++)
  {
    printDigit(passcode[i], false);
    delay(400);
    shiftClear(S_DIGIT);
    delay(400);
  }
}

// possibly combine both clears; and have a if flag
// NOTE: i don't have to test for S_BOTH because d1 and d2 are, by default, already 0.
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

void LoseFunc()
{
  shiftClear(S_DIGIT);
  unsigned long loseCurrentMillis = millis();
  if (loseSwapFlag == false)
  {
    dataResult = 0b10101010;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, dataDigit);
    shiftOut(dataPin, clockPin, dataResult);
    digitalWrite(latchPin, 1);
  }
  else
    shiftClear(S_RESULT);
  if (loseCurrentMillis - losePreviousMillis > interval)
  {
    losePreviousMillis = loseCurrentMillis;
    loseSwapFlag = (loseSwapFlag == false ? true : false);
  }
}

void WinFunc()
{
  unsigned long winCurrentMillis = millis();
  shiftClear(S_DIGIT);
  if (winSwapFlag == false)
  {
    dataResult = 0b01010101;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, dataDigit);
    shiftOut(dataPin, clockPin, dataResult);
    digitalWrite(latchPin, 1);
  }
  else
    shiftClear(S_RESULT);
  if (winCurrentMillis - winPreviousMillis > interval)
  {
    winPreviousMillis = winCurrentMillis;
    winSwapFlag = (winSwapFlag == false ? true : false);
  }
}

void showWin()
{
  byte winAnim[] = {
    0b10000001,
    0b11000000,
    0b01000010,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00010010,
    0b00000011
  };

  for (int j = 0; j < 6; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, winAnim[i]);
      shiftOut(dataPin, clockPin, dataResult);
      digitalWrite(latchPin, 1);
      delay(90);
      shiftClear(S_DIGIT);
    }
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
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0b00000000);
    shiftOut(dataPin, clockPin, dataResult);
    digitalWrite(latchPin, 1);
  }
}

void printNumLeft()
{
  int disp_tries = 0;
  char ten[2];

  disp_tries = abs(max_tries - num_tries);
  ten[0] = (max_tries > 9 ? (disp_tries / 10) + 48 : '0');
  ten[1] = (disp_tries % 10) + 48;
  printMultDigit(ten, 2);
}

void printChar(char k, int clear_flag)
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, digitArray[k - 48]);
  shiftOut(dataPin, clockPin, dataResult);
  digitalWrite(latchPin, 1);
  if (clear_flag == true)
  {
    delay(500);
    shiftClear(S_DIGIT);
  }
}

void printMultDigit(char *text, int text_len)
{
  for (int i = 0; i < text_len; i++)
  {
    printChar(text[i], true);
    delay(100);
  }
}

void printDigit(int num, int decimalFlag)
{
  byte Digit = (num > 9 ? 0b00000000 : digitArray[num]);
  // print decimal
  //     543210
  // 0b00100000
  if (decimalFlag == true)    bitSet(Digit, 5);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, Digit);
  shiftOut(dataPin, clockPin, 0b00000000);
  digitalWrite(latchPin, 1);
}

void swap(int *a, int *b)
{
  int temp = *a;

  *a = *b;
  *b = temp;
}

int prevDigit = 10;

void printR2()
{
  float tmpT, tmpH;
  int tenth, hundth;
  int d = 500;

  printDigit(R2 / 10, false);
  delay(d);
  printDigit((int)R2 % 10, true);
  delay(d);
  tmpT = R2 * 10;
  tenth = (int)tmpT % 10;
  printDigit(tenth, false);
  delay(d);
  tmpH = R2 * 100;
  hundth = (int)tmpH % 10;
  printDigit(hundth, false);
  delay(d);
}

void decodePass()
{
  Serial.println();
  Serial.print("[ ");
  for (int i = 0; i < ANS_NUM; i++)
  {
    switch(passcode[i])
    {
    case LED_SR:      Serial.print("square_red ");   break;
    case LED_BG:      Serial.print("big_green ");    break;
    case LED_BB:      Serial.print("big_blue ");     break;
    case LED_BY:      Serial.print("big_yellow ");   break;
    case LED_BR:      Serial.print("big_red ");      break;
    case LED_LW:      Serial.print("little_white "); break;
    }
  }
  Serial.println(" ]");
}

void printColor(int color)
{
  switch (color)
  {
    case LED_SR:      Serial.print("square_red");   break;
    case LED_BG:      Serial.print("big_green");    break;
    case LED_BB:      Serial.print("big_blue");     break;
    case LED_BY:      Serial.print("big_yellow");   break;
    case LED_BR:      Serial.print("big_red");      break;
    case LED_LW:      Serial.print("little_white"); break;
    default:          Serial.print("nothing");      break;
  }
}
