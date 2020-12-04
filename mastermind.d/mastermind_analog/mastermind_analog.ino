/*
  Mastermind using LEDs

 Wiring:
 5v---*---10k----[LED]----GND
 * = analog connection
                           Arduino / raspberry pi
     +-----+               +  +   +
1out |     | Vin           |  |   |
2out |     | 0out          |  |   |
3out |     | data----------+  |   |
4out |     | Gnd              |   |
5out |     | latch--+---------+   |
6out |     | clock-----+----------+
7out |     | vin    |  |
 gnd |     | ser--+ |  |
     +-----+      | |  |
                  | |  |
     +-----+      | |  |
1out |     | Vin  | |  |
2out |     | 0out | |  |
3out |     | data-+ |  |
4out |     | Gnd    |  |
5out |     | latch--+  |
6out |     | clock-----+
7out |     | vin
 gnd |     | serial
     +-----+

 
 7-segment LED Pins:
        ____
 A  1--|    |--8  H        --    H
 B  2--|    |--7  G       |  | A   G
 X     |    |     X        --    B
  GND--|    |     X       |  | C   E
 X     |    |     X        -- .  D   F (.)
 C  3--|    |--6  F (.)
 D  4--|____|--5  E
*/
#include <string.h>

#define NUM_LED 7
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
  LED_OFF
} status_enum;

typedef enum {
  LED_0,
  LED_1,
  LED_2,
  LED_3,
  LED_4,
  LED_5,
  LED_6
} led_enum;

int alertPin = LED_BUILTIN;

int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

int buttonPinA = 5;
int buttonPinB = 6;
int buttonPinC = 7;
int testButtonPin = 12;

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

int  testButtonState; // current reading from the input pin
int  testLastButtonState = LOW; // previous reading from the input pin
long testLastDebounceTime = 0; // last time the output pin was toggled

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

float highs[] = { -1,   -1,   -1,   -1,   -1,   -1,   -1 };   // high values
float lows[] =  { 1000, 1000, 1000, 1000, 1000, 1000, 1000 }; // low

int dataDigit;
int dataResult;

byte alphabetArray[] = {
  0b11010111, // A
  0b00011111, // B
  0b10001101, // C
  0b01011110, // D
  0b10001111, // E
  0b10000111, // F
  0b10011101, // G
  0b01010111, // H
  0b01010000, // I
  0b01011100, // J
  0b01110111, // K (H.)
  0b00001101, // L
  0b00110110, // M (n.) c b e f
  0b00010110, // N (n)  c b e
  0b11011101, // O
  0b11000111, // P
  0b11111101, // Q (O.)
  0b00000110, // R (r)
//  0b11100111, // R (P.)
  0b10011011, // S
  0b11010000, // T
  0b01011101, // U
  0b00011100, // V
  0b00111100, // W
  0b01010101, // X
  0b01011011, // Y
  0b11001110, // Z (2)
  0b00000000 // space
};

byte digitArray[] = {
  0b11011101, // 0
  0b01010000, // 1
  0b11001110, // 2
  0b11011010, // 3
  0b01010011, // 4
  0b10011011, // 5
  0b10011111, // 6
  0b11010000, // 7
  0b11011111, // 8
  0b11011011, // 9
  0b00100000  // .
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
  int test_reading;
  int readingA;
  int readingB;
  int stopFlag = false;
  int cur_led = LED_0;

  randomSeed(analogRead(A0));
#ifdef DEBUG
  Serial.begin(9600);
#endif
  for (int i = 0; i < ANS_NUM; i++)
    answer[i] = result[i] = LED_OFF;
  for (int i = 0; i < NUM_LED; i++)
  {
    used[i] = 0;
    leds[i] = cur_led++;
  }
  pinMode(testButtonPin, INPUT_PULLUP);
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinC, INPUT);
  pinMode(alertPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  shiftClear(S_BOTH);
  delay(100);
  create_passcode();
//  blip(3, 100);
//  blinkAll_2Bytes(4, 250);
//  zero2nine();
//  green2red(400);
  /*
  =================
    Calibration
  =================
  */
  shiftClear(S_RESULT);
//  printMessage("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 500);
//  printMessage("INSERT LED", 300);
//  delay(300);
//  printMessage("CAL", 250);
  re_calibrate();
  // Pause giving time to remove the LEDs
  printMessage("GO", 400);
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
  blip(3, 100);
  shiftClear(S_DIGIT); // S_DIGIT, S_RESULT, or S_BOTH
}
/*
#ifdef DEBUG
  Serial.println("[setup] ------- Start -------");
  Serial.println("-=-=-=-=-=-=-");
  Serial.print("[setup] Highs: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(highs[i]);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.print("[setup] Lows: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(lows[i]);
    Serial.print(" ");
  }
  Serial.println("-=-=-=-=-=-=-");
  Serial.println("[setup] Ranges for passcode:");
  for (int i = 0; i < ANS_NUM; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(lows[passcode[i]]);
    Serial.print(" ");
    Serial.println(highs[passcode[i]]);
  }
  Serial.println("[setup] ------- End -------");
#endif
*/

void loop()
{
  int readingA;
  int readingB;
  int readingC;
  int testReading;
  int breakWhile = false; // used to wait until all LEDs are put in

  if (loopFlag == true)
  {
#ifdef DEBUG
//    Serial.println("[loop] ==== Begin Loop ====");
#endif
    loopFlag = false;
  }
 /*
    Steps:
    1) for loop; read in each analog
    2) if all each one has Vout < 5, then all have been inserted
    3)
  */
#ifdef DEBUG
  testReading = digitalRead(testButtonPin);
  if (testReading != testLastButtonState)
    testLastDebounceTime = millis();
  if ((millis() - testLastDebounceTime) > debounceDelay)
  {
    if (testReading != testButtonState)
    {
      testButtonState = testReading;
      if (testButtonState == LOW)
      {
//        re_calibrate();
        viewResultFlag = true;
      }
    }
  }
  testLastButtonState = testReading;
#endif
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
        {
          printPass();
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
          green2red(400);
          shiftClear(S_RESULT);
          zero2nine();
          shiftClear(S_DIGIT);
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
//          Serial.print("ans: ");
          for (int i = 0; i < ANS_NUM; i++)
          {
            // this MUST be a number between 0 and 6
            answer[i] = testPin(i);
//            Serial.print(answer[i]);
//            Serial.print(" ");
            delay(100);
          }
//          Serial.println("");
          if (answer[0] != LED_OFF && answer[1] != LED_OFF && answer[2] != LED_OFF && answer[3] != LED_OFF)
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
    {
      result[i] = 1;
    }
  }
#ifdef DEBUG
  if (viewResultFlag == true)
  {
    Serial.println("");
    Serial.println("i | answer[i] | passcode[i]");
    for (int i = 0; i < ANS_NUM; i++)
    {
      Serial.print(i);
      Serial.print(" | ");
      Serial.print(answer[i]);
      Serial.print(" | ");
      Serial.println(leds[passcode[i]]);
    }
    Serial.println("");
    /*
    Serial.print("[Result: ");
    for (int i = 0; i < ANS_NUM; i++)
    {
      Serial.print(result[i]);
      Serial.print(" ");
    }
    Serial.println("]");
    */
    viewResultFlag = false;
  }
#endif
  if (result[0] == 1 && result[1] == 1 && result[2] == 1 && result[3] == 1)
  {
    winFlag = true;
  }
  if (winFlag == true)
  {
    WinFunc();
  }
  delay(500);
}


int new_color = LED_0;

void calibrate(int firstTurn)
{
  int pos = 4;
  int ending = 3;

  digitalWrite(alertPin, HIGH);
  if (firstTurn == true)
  {
    pos = 0;
    ending = 4;
  }
#ifdef DEBUG
  Serial.println("--== Begin Calibrate ==--");
#endif
  for (int i = 0; i < ending; i++)
  {
    for (int j = 0; j < 100; j++)
    {
      raw = analogRead(i);
      Vout = (5.0 / 1023.0) * raw;
      buffer = (Vin / Vout) - 1;
      R2 = R1 / buffer;
      if (lows[pos] > R2)
        lows[pos] = R2;
      if (highs[pos] < R2)
        highs[pos] = R2;
      delay(10);
//      leds[pos] = new_color++;
    }
#ifdef DEBUG
    Serial.print("i: ");
    Serial.print(i);
    Serial.print(" pos: ");
    Serial.print(pos);
    Serial.print(" lows[pos]: ");
    if (lows[pos] < 10)
      Serial.print("0");
    Serial.print(lows[pos]);
    Serial.print(" highs[pos]: ");
    if (highs[pos] < 10)
      Serial.print("0");
    Serial.println(highs[pos]);
#endif
    pos++;
  }
  blip(4, 100);
#ifdef DEBUG
  Serial.println("--== End Calibrate ==--");
#endif
  digitalWrite(alertPin, LOW);
}
/*
  // check for dupiclates
  for (int i = 0; i < NUM_LED; i++)
  {
    if (check4dup(i) == true)
    {
      for (int j = 0; j < 3; j++)
      {
        printMessage("ERROR", 250);
        delay(300);
        printDigit(i, false);
      }
    }
  }
*/

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
      printR2();
    Serial.println("-=-=-=-=-=-= TEST_PIN =-=-=-=-=-=-");
    for (int j = 0; j < NUM_LED; j++)
    {
        Serial.println("");
      if (R2 >= lows[j] && R2 <= highs[j])
      {
        res = j;
        if (pin == 0)          printDigit(res, false);
//        res = leds[j];
        break;
      }
      else
      {
        Serial.print("no-");
      }
        Serial.print("match j: ");
        Serial.print(j);
        Serial.print(" pin: ");
        Serial.print(pin);
        Serial.print(" R2: ");
        Serial.print(R2);
        Serial.print(" leds[j]: ");
        Serial.print(leds[j]);
        Serial.print(" high: ");
        Serial.print(highs[j]);
        Serial.print(" low: ");
        Serial.println(lows[j]);
    }
    Serial.println("=====================");
  }
  else
    res = LED_OFF;
  return res;
}
    /*
         if (R2 >=  4.98 && R2 <=  5.16) res = LED_R_S;
    else if (R2 >=  5.34 && R2 <=  5.54) res = LED_R_C;
    else if (R2 >=  5.62 && R2 <=  5.69) res = LED_G_B;
    else if (R2 >=  5.73 && R2 <=  6.10) res = LED_Y_C;
    else if (R2 >=  8.30 && R2 <=  9.00) res = LED_G_C;
    else if (R2 >= 10.20 && R2 <= 10.66) res = LED_B_C;
    else if (R2 >= 10.72 && R2 <= 11.40) res = LED_W_B;
    */

void changeBit(int pos, int color)
{
  // pos = 0 - 3
  // color = LED_G, LED_R, or LED_OFF
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
    case LED_OFF:
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
  int Lights[] = { LED_G, LED_R };

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
  byte b;
  for (int i = 0; i < strlen(message); i++)
  {
    b = (message[i] == 32 ? alphabetArray[26] : alphabetArray[message[i] - 65]);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, b);
    shiftOut(dataPin, clockPin, dataResult);
    digitalWrite(latchPin, 1);
    delay(d);
    shiftClear(S_DIGIT);
    delay(100);
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
  if (decimalFlag == true)
    bitSet(Digit, 5);
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

int check4dup(int num)
{
  int ret = false;
  int h_l = true;

  for (int i = 0; i < NUM_LED; i++)
  {
    if (h_l == true)
    {
      if ((i != num) && (highs[i] == highs[num]))
      {
          ret = true;
          break;
      }
    }
    else
    {
      if ((i != num) && (lows[i] == lows[num]))
      {
          ret = true;
          break;
      }
    }
  }
  return ret;
}

/*
int checkButton(int pin, int lbs, int bs, long ldt)
{
  int reading;
  int ret = false;

    reading = digitalRead(pin);
    if (reading != lastButtonStateA)
      lastDebounceTimeA = millis();
    if ((millis() - lastDebounceTimeA) > debounceDelay)
    {
      if (reading != buttonStateA)
      {
        buttonStateA = reading;
        if (buttonStateA == LOW)
          ret = true;
      }
    }
    lastButtonStateA = reading;
    return ret;
}
*/

int prevDigit = 10;

void re_calibrate()
{
  int stopFlag = false;
  int curLed = 0;
  int readingA;
  int readingB;

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
          calibrate((curLed++ == 0 ? true : false));
      }
    }
    lastButtonStateA = readingA;
    // attempt to avoid constantly printing the digit.
    if (curLed != prevDigit && (abs(curLed - 2) != 0))
    {
      printDigit(abs(curLed - 2), false);
      prevDigit = curLed;
    }
    if (curLed == 2)
      stopFlag = true;
//    blip(1, 300);
  }
}

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

int fromSerial()
{
  int retval = 0;
  if (Serial.available())
  {
    int val = Serial.read();
    switch (val)
    {
      case 'r':
        printR2();
        retval = 1;
        break;
    }
  }
  return retval;
}

// XXXXXX

#if 0
/*
   ===================================================
   =                 Code with LEDs:                 =
   ===================================================
*/
    switch (passcode[i]) {
      case LED_B: useB = 1; break;
      case LED_R_C: useR = 1; break;
    }
  // winner
  if (winner == 1 || (passcode[0] == answer[0] && passcode[1] == answer[1] && passcode[2] == answer[2] && passcode[3] == answer[3]))
    winfunc();
  else {
    // test if nothing is plugged in
    for (i = 0; i < 4; i++)
      if (answer[i] == LED_0_0) result[i] = LED_0_0;
    // Turn on green
    if (passcode[0] == answer[0] || passcode[1] == answer[1] || passcode[2] == answer[2] || passcode[3] == answer[3]) {
      if (passcode[0] == answer[0]) result[0] = LED_G_C;
      // ...
      if (passcode[3] == answer[3]) result[3] = LED_G_C;
    }
    // find colors
    for (i = 0; i < 4; i++) {
      switch (answer[i]) {
        case LED_B_C: if (result[i] != LED_G_C && useB_C == 1) result[i] = LED_R_C; break;
        case LED_R_C: if (result[i] != LED_G_C && useR_C == 1) result[i] = LED_R_C; break;
      }
    }
    for (i = 0; i < 4; i++) {
      switch (answer[i]) {
        case LED_B_C: if (useB == 0) result[i] = LED_R_C; break;
        case LED_R_C: if (useR == 0) result[i] = LED_R_C; break;
      }
    }
    for (i = 3; i > -1; i--) changeBit(i, result[i]);
    delay(1000);
  }
}
/*
   ===================================================
   =                 Code with keypad:               =
   ===================================================
*/
    shiftClear(S_LED);
    if (loseFlag == false && winFlag == false) {
      correct_index = correct_counter = 0;
      for (int i = 0; i < ANS_NUM; i++) {
        result[i] = LED_0;
        answer[i] = 'E';
        changeBit(i, LED_0);
        shiftClear(S_RESULT);
      }
      if (key == '*') {
        shuffleFlag = true;
        printNumLeft();
        blip(2, 100);
      }
    }
    if (key == '#') {
      shiftClear(S_BOTH);
      num_tries = correct_index = correct_counter = 0;
      blip(3, 100);
      winFlag = loseFlag = false;
      resetFlag = true;
      for (int i = 0; i < ANS_NUM; i++) result[i] = LED_0;
      for (int i = 0; i < NUM_LED; i++) used[i] = 0;
      create_passcode();
    }
  }
    if (firstFlag == true) {
      firstFlag = false;
      blip(2, 150);
    }
    if (key != NO_KEY && winFlag == false && (key != '*' && key != '#'))
      answer[correct_index++] = key;
    // we've gotten all the digits we need
    if (correct_index == ANS_NUM && winFlag == false) {
      correct_index = 0;
      // test for a correct code
      for (int i = 0; i < ANS_NUM; i++) {
        if (passcode[i] == answer[i]) {
          correct_counter++;
          result[i] = LED_G;
        }
      }
      if (correct_counter >= ANS_NUM) winFlag = true;
      else {
        correct_counter = 0;
        for (int i = 0; i < ANS_NUM; i++)
          if ((result[i] != LED_G) && used[(answer[i] - 48)] == 1) result[i] = LED_R;
        num_tries++;
      }
      // Test to see if you lost
      if (num_tries > max_tries) {
        loseFlag = true;
        winFlag = false;
      }
    }
    if (winFlag == false && loseFlag == false) {
      if (result[ANS_NUM-1] != LED_OFF) {
        // Shuffle the result
        if (shuffleFlag == true) {
          for (int i = ANS_NUM - 1; i > 0; i--) swap(&result[i], &result[random(0, i)]);
          for (int i = 0; i < ANS_NUM; i++) changeBit(i, result[i]);
          shuffleFlag = false;
        }
      }
    }
  }
  delay(100);
}

void printLed(int c)
{
  switch (c)
  {
    case LED_0:
      Serial.print("zero");
      break;
    case LED_1:
      break;
    case LED_2:
      break;
    case LED_3:
      break;
    case LED_4:
      break;
    case LED_5:
      break;
    case LED_6:
      break;
  }
}

#endif

