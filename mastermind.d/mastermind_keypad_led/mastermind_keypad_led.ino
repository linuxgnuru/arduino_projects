/*
 Mastermind using a 4 x 4 keypad
 7 segment LED + shift register (74HC595) + keypad
 wiring for keypad: with pad facing you and starting from left to right; 

 +---+---+---+---+                shift register Pins:
 | 1 | 2 | 3 | A |                     +--------+
 +---+---+---+---+               1out -+ 1   16 +- Vin
 | 4 | 5 | 6 | B |               2out -+ 2   15 +- 0out
 +---+---+---+---+               3out -+ 3   14 +- data (blue)
 | 7 | 8 | 9 | C |               4out -+ 4   13 +- ground
 +---+---+---+---+               5out -+ 5   12 +- latch (green + 1u cap if first)
 | * | 0 | # | D |               6out -+ 6   11 +- lock (yellow)
 +---+---+---+---+               7out -+ 7   10 +- Vin
 |2|3|4|5|6|7|8|9| (pins used)    gnd -+ 8    9 +- serial out
 +-+-+-+-+-+-+-+-+                     +--------+
  | | | | | | | |
  + + + + + + + +

7-segment LED Pins:
        ____
 A  1--|    |--8  H        --    H
 B  2--|    |--7  G       |  | A   G
 X     |    |     X        --    B
  GND--|    |     X       |  | C   E
 X     |    |     X        -- .  D   F (.)
 C  3--|    |--6  F (.)
 D  4--|____|--5  E

        HGFEDCBA
0out = 0b10000000
1out = 0b01000000
2out = 0b00100000
3out = 0b00010000
4out = 0b00001000
5out = 0b00000100
6out = 0b00000010
7out = 0b00000001
*/

#include <Keypad.h>

typedef enum {
  LED_R, // red
  LED_Y, // yellow
  LED_G, // green
  LED_0  // nothing
} 
led_enum;

// Defines
//#define DEBUG 1
#define NUM_LEDS 5
#define MAX_DIG 10 // 10 = 0-9, 14 = 0-9, A-D

long winPreviousMillis = 0;
long losePreviousMillis = 0;

long interval = 300;

int max_tries = 10;

/*
 * Shift register variables:
 */

int resultDataPin  = 10; // Pin connected to DS of 74HC595 - blue wire
int resultLatchPin = 11; // Pin connected to ST_CP of 74HC595 - green wire
int resultClockPin = 12; // Pin connected to SH_CP of 74HC595 - yellow wire

// Use analog input pins as digital output/inputs
// A0 is reserved for use by the random seed generator
int digitClockPin = A1; // Pin connected to SH_CP of 74HC595 - yellow wire
int digitLatchPin = A2; // Pin connected to ST_CP of 74HC595 - green wire
int digitDataPin = A3;  // Pin connected to DS of 74HC595 - blue wire

/*
 * Keypad code:
 */
const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = { 2, 3, 4, 5 }; //connect to row pinouts 
byte colPins[COLS] = { 6, 7, 8, 9 }; //connect to column pinouts

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte digitArray[] = {
//  HG EDC A
  0b11011101, // 0
//   G E    
  0b01010000, // 1 
//  HG  DCB 
  0b11001110, // 2
//  HG ED B 
  0b11011010, // 3
//   G E  BA
  0b01010011, // 4
//  H  ED BA
  0b10011011, // 5
//  H  EDCBA
  0b10011111, // 6
//  HG E    
  0b11010000, // 7
//  HG EDCBA
  0b11011111, // 8
//  HG ED BA
  0b11011011, // 9
//  HGFE CBA
  0b11110111, // A
//  HGFEDCBA
  0b11111111, // B
//  H F DC A
  0b10101101, // C
//  HGFEDC A
  0b11111101, // D
//   GFE CBA
  0b01110111, // *
//  H FE CB 
  0b10110110  // #
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//const char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D' };
const char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D' };

// Flags
int rFlag = false; // is this used?
int resetFlag = false; // is this used?
int firstFlag = true;
int winFlag = false;
int loseFlag = false;
int winSwapFlag = false;
int loseSwapFlag = false;

// this will be randomly generated
char passcode[NUM_LEDS];
// users input
char answer[NUM_LEDS];
// result
int result[NUM_LEDS];
// what digits were picked
int used[MAX_DIG];

// for shift register leds:
int data1;
int data2;
int correct_index = 0;
int correct_counter = 0;
int num_tries = 0;

void setup()
{
  firstFlag = true;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(resultLatchPin, OUTPUT);
  pinMode(digitLatchPin, OUTPUT);
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    answer[i] = 'E';
    result[i] = LED_0;
  }
  for (int i = 0; i < MAX_DIG; i++)
    used[i] = 0;
  randomize();
  // The following is done in randomize (I think)
//   for (int i = 0; i < MAX_DIG; i++) for (int j = 0; j < NUM_LEDS; j++) if (letters[i] == pass[j]) used[i] = 1;
#ifdef TESTLED
  // g2r - green to red
  g2r(150);
  printMultDigit(10, "1234567890");
#endif
  digitClear();
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY && (key != '*' && key != '#'))
  {
    digitClear();
    printSingleDigit(key);
  }
  if (key != NO_KEY && (key == '*' || key == '#'))
  {
    digitClear();
    if (loseFlag == false && winFlag == false)
    {
      correct_index = correct_counter = 0;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        result[i] = LED_0;
        answer[i] = 'E';
        changeBit(i, LED_0);
      }      
      if (key == '*')
      {
        blip(2, 100);
        rFlag = true;
        printNumLeft();
      }
    }
    if (key == '#')
    {
      correct_index = correct_counter = 0;
      blip(3, 100);
      winFlag = false;
      loseFlag = false;
      resetFlag = true;
      num_tries = 0;
      for (int i = 0; i < NUM_LEDS; i++)
        result[i] = LED_0;
      for (int i = 0; i < MAX_DIG; i++)
        used[i] = 0;
      randomize();
    }
  }
  // Special keys
  if (key != NO_KEY && (key == 'A' || key == 'B' || key == 'C' || key == 'D'))
  {
    correct_index = correct_counter = 0;
    switch (key)
    {
      // display number of tries
      case 'A':
        printNumLeft();
        break;
      // display the password
      case 'B':
        printMultDigit(5, passcode);
        break;
      case 'C':
        break;
      // display demo
      case 'D':
//        printMultDigit(10, "1234567890");
        g2r(150);
        digitClear();
        break;
    }
  }
  else
  {
    if (firstFlag == true)
    {
      firstFlag = false;
      blip(2, 150);
    }
    if (key != NO_KEY && winFlag == false && (key != '*' && key != '#'))
    {
      //blip(1, 100);
      answer[correct_index++] = key;
#ifdef DEBUG
      Serial.print(num_tries);
      Serial.print(" - ");
      Serial.print("[");
      Serial.print(correct_index - 1);
      Serial.print(":");
      Serial.print(key);
      Serial.println("]");
#endif
    }
    // we've gotten all the digits we need
    if (correct_index == NUM_LEDS && winFlag == false)// && (key != '*' && key != '#'))
    {
      correct_index = 0;
      // test for a correct code
      for (int i = 0; i < NUM_LEDS; i++)
      {
        if (passcode[i] == answer[i])
        {
          correct_counter++;
          result[i] = LED_G;
        }
      }
      if (correct_counter >= NUM_LEDS)
        winFlag = true;
      else
      {
        correct_counter = 0;
        for (int i = 0; i < NUM_LEDS; i++)
        {
          if ((result[i] != LED_G) && used[whatchar(answer[i])] == 1)
            result[i] = LED_Y;
          if (result[i] == LED_0)
            result[i] = LED_R;
        }
        num_tries++;
      } // end else (not winner)
      // Test to see if you lost
      if (num_tries > max_tries)// && loserFlag == true)
      {
        loseFlag = true;
        winFlag = false;
      }
    } // end if (NUM_LEDS buttons check
    // lost
    if (loseFlag == true)
    {
      digitClear();
      unsigned long loseCurrentMillis = millis();
      if (loseSwapFlag == false)
      {
        data1 = 0b00100100;
        data2 = 0b10010010;
        digitalWrite(resultLatchPin, 0);
        shiftOut(resultDataPin, resultClockPin, data1);
        shiftOut(resultDataPin, resultClockPin, data2);
        digitalWrite(resultLatchPin, 1);
//        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        data1 = 0b01001001;
        data2 = 0b00100100;
        digitalWrite(resultLatchPin, 0);
        shiftOut(resultDataPin, resultClockPin, data1);
        shiftOut(resultDataPin, resultClockPin, data2);
        digitalWrite(resultLatchPin, 1);
//        digitalWrite(LED_BUILTIN, LOW);
      }
      if (resetFlag == true || loseCurrentMillis - losePreviousMillis > interval)
      {
        if (resetFlag == false)
          losePreviousMillis = loseCurrentMillis;
        loseSwapFlag = (loseSwapFlag == false ? true : false);
      }
    }
    // win
    if (winFlag == true)
    {
      unsigned long winCurrentMillis = millis();
      if (winSwapFlag == false)
      {
        data1 = 0b10010010;
        data2 = 0b01001000;
        digitalWrite(resultLatchPin, 0);
        shiftOut(resultDataPin, resultClockPin, data1);
        shiftOut(resultDataPin, resultClockPin, data2);
        digitalWrite(resultLatchPin, 1);
//        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        data1 = 0b01001001;
        data2 = 0b00100100;
        digitalWrite(resultLatchPin, 0);
        shiftOut(resultDataPin, resultClockPin, data1);
        shiftOut(resultDataPin, resultClockPin, data2);
        digitalWrite(resultLatchPin, 1);
//        digitalWrite(LED_BUILTIN, LOW);
      }
      if (resetFlag == true || winCurrentMillis - winPreviousMillis > interval)
      {
        if (resetFlag == false)
          winPreviousMillis = winCurrentMillis;
        winSwapFlag = (winSwapFlag == false ? true : false);
      }
    }
    if (winFlag == false && loseFlag == false)
    {
      if (result[0] != 3)// && (key != '*' && key != '#'))
      {
#ifdef DEBUG
        /* MASSIVE DEBUGGING STUFF */
        if (Serial.available())
        {
          int val = Serial.read();
          switch (val)
          {
          case '?': case 'H': case 'h':
            Serial.println("");
            Serial.println("help");
            Serial.println("----");
            Serial.println("p - passcode / answer");
            Serial.println("r - result (just digits)");
            Serial.println("c - result (color name)");
            Serial.println("u - used");
            Serial.println("l - letters / used");
            Serial.println("h/? - this list");
            break;
          case 'P': case 'p': case 'R': case 'r':
            Serial.println("");
            Serial.print("pass: [");
            for (int i = 0; i < NUM_LEDS; i++)
              Serial.print(passcode[i]);
            Serial.print("] answer: [");
            for (int i = 0; i < NUM_LEDS; i++)
              Serial.print(answer[i]);
            Serial.println("]");
            if (val == 'R' || val == 'r')
            {
              Serial.print("result: ");
              for (int i = 0; i < NUM_LEDS; i++)
              {
                Serial.print(result[i]);
                Serial.print(" ");
              }
              Serial.println("");
            }
            break;
          case 'C': case 'c':
            Serial.print("result: ");
            for (int i = 0; i < NUM_LEDS; i++)
            {
              Serial.print(print_color(result[i]));
              Serial.print(" ");
            }
            Serial.println("");
            break;
          case 'U': case 'u':
            Serial.println("");
            Serial.println("used:");
            for (int i = 0; i < MAX_DIG; i++)
            {
              Serial.print(i);
              Serial.print(" ");
            }
            Serial.println("");
            for (int i = 0; i < MAX_DIG; i++)
            {
              Serial.print(used[i]);
              Serial.print(" ");
            }
            Serial.println("");
            break;
          case 'L': case 'l':
            Serial.println("");
            Serial.println("letters:");
            for (int i = 0; i < MAX_DIG; i++)
            {
              Serial.print("[");
              if (i < 10)
                Serial.print("0");
              Serial.print(i);
              Serial.print("]: ");
              Serial.print(letters[i]);
              Serial.print(" ");
              Serial.println(used[i]);
            }
            Serial.println("");
            break;
          }
        }
#endif
        for (int i = 0; i < NUM_LEDS; i++)
          changeBit(i, result[i]);
      }
      else
      {
        for (int i = 0; i < NUM_LEDS; i++)
          changeBit(i, LED_0);
      }
    }
  }
  delay(100);
}

void randomize()
{
  int ran;
//  int break_flag = false;
  int cur_index = 0;

  randomSeed(analogRead(A0));
  for (int i = 0; i < MAX_DIG; i++)
    used[i] = 0;
  for (cur_index = 0; cur_index < NUM_LEDS; cur_index++)
  {
    ran = random(0, MAX_DIG);
    used[ran] = 1;
    passcode[cur_index] = letters[ran];
  }

/*
  while (break_flag == false)
  {
    ran = random(0, MAX_DIG);
    if (used[ran] == 0)
    {
      used[ran] = 1;
      pass[idx++] = letters[ran];
    }
    if (idx == NUM_LEDS)
      break_flag = true;
  }
  */
#ifdef DEBUG
  Serial.println("--NEW--");
  Serial.print("[PASSCODE: ");
  for (int i = 0; i < NUM_LEDS; i++)
    Serial.print(passcode[i]);
  Serial.println("]");
#endif
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
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

void g2r(int d)
{
  int d1 = 0b00000000;
  int d2 = 0b00000000;
  /*
    data1      data2
    GYR GYR GY|R GYR GYR x
    765 432 10|7 654 321 0
    000 000 00|0 000 000 0 // all off
    100 100 10|0 100 100 0 // all green d1 = 7, 4, 1; d2 = 6, 3
    010 010 01|0 010 010 0 // all yellow d1 = 6, 3, 0; d2 = 5, 2
    001 001 00|1 001 001 0 // all red d1 = 5, 2; d2 = 7, 4
    100 010 00|1 100 010 0 // green yellow red green yellow
  */
  // loop through all printable digits for 7 segment LED
  for (int i = 0; i < MAX_DIG + 4; i++)
  {
    digitalWrite(digitLatchPin, 0);
    shiftOut(digitDataPin, digitClockPin, digitArray[i]);
    digitalWrite(digitLatchPin, 1);
    delay(250);
  }
  for (int i = 7; i > -1; i--)
  {
    bitSet(d1, i); // d1 |= 1 << i;
    digitalWrite(resultLatchPin, 0);
    shiftOut(resultDataPin, resultClockPin, d1);
    shiftOut(resultDataPin, resultClockPin, d2);
    digitalWrite(resultLatchPin, 1);
    delay(d);
    if (i != 5 && i != 2)
    {
      bitClear(d1, i); // d1 &= ~(1 << i);
      digitalWrite(resultLatchPin, 0);
      shiftOut(resultDataPin, resultClockPin, d1);
      shiftOut(resultDataPin, resultClockPin, d2);
      digitalWrite(resultLatchPin, 1);
    }
  }
  d1 = 0b00100100;
  for (int i = 7; i > 0; i--)
  {
    bitSet(d2, i); // d2 |= 1 << i;
    digitalWrite(resultLatchPin, 0);
    shiftOut(resultDataPin, resultClockPin, d1);
    shiftOut(resultDataPin, resultClockPin, d2);
    digitalWrite(resultLatchPin, 1);
    delay(d);
    if (i != 7 && i != 4 && i != 1)
    {
      bitClear(d2, i); // d2 &= ~(1 << i);
      digitalWrite(resultLatchPin, 0);
      shiftOut(resultDataPin, resultClockPin, d1);
      shiftOut(resultDataPin, resultClockPin, d2);
      digitalWrite(resultLatchPin, 1);
    }
  }
}

void blip(int times, int d)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}

void changeBit(int pos, int color)
{
  switch (pos)
  {
  case 0: // data1: 7 6 5
    switch (color)
    {
    case LED_G:
      bitSet(data1, 7);
      bitClear(data1, 6);
      bitClear(data1, 5);
      break;
    case LED_Y:
      bitClear(data1, 7);
      bitSet(data1, 6);
      bitClear(data1, 5);
      break;
    case LED_R:
      bitClear(data1, 7);
      bitClear(data1, 6);
      bitSet(data1, 5);
      break;
    case LED_0:
      for (int i = 5; i < 8; i++)
        bitClear(data1, i);
      break;
    }
    break;
  case 1: // data1: 4 3 2
    switch (color)
    {
    case LED_G:
      bitSet(data1, 4);
      bitClear(data1, 3);
      bitClear(data1, 2);
      break;
    case LED_Y:
      bitClear(data1, 4);
      bitSet(data1, 3);
      bitClear(data1, 2);
      break;
    case LED_R:
      bitClear(data1, 4);
      bitClear(data1, 3);
      bitSet(data1, 2);
      break;
    case LED_0:
      for (int i = 2; i < 5; i++)
        bitClear(data1, i);
      break;
    }
    break;
  case 2: // data1: 1 0, data2: 7
    switch (color)
    {
    case LED_G:
      bitSet(data1, 1);
      bitClear(data1, 0);
      bitClear(data2, 7);
      break;
    case LED_Y:
      bitClear(data1, 1);
      bitSet(data1, 0);
      bitClear(data2, 7);
      break;
    case LED_R:
      bitClear(data1, 1);
      bitClear(data1, 0);
      bitSet(data2, 7);
      break;
    case LED_0:
      bitClear(data1, 1);
      bitClear(data1, 0);
      bitClear(data2, 7);
      break;
    }
    break;
  case 3: // data2: 6 5 4
    switch (color)
    {
    case LED_G:
      bitSet(data2, 6);
      bitClear(data2, 5);
      bitClear(data2, 4);
      break;
    case LED_Y:
      bitClear(data2, 6);
      bitSet(data2, 5);
      bitClear(data2, 4);
      break;
    case LED_R:
      bitClear(data2, 6);
      bitClear(data2, 5);
      bitSet(data2, 4);
      break;
    case LED_0:
      for (int i = 4; i < 7; i++)
        bitClear(data2, i);
      break;
    }
  case 4: // data2: 3 2 1
    switch (color)
    {
    case LED_G:
      bitSet(data2, 3);
      bitClear(data2, 2);
      bitClear(data2, 1);
      break;
    case LED_Y:
      bitClear(data2, 3);
      bitSet(data2, 2);
      bitClear(data2, 1);
      break;
    case LED_R:
      bitClear(data2, 3);
      bitClear(data2, 2);
      bitSet(data2, 1);
      break;
    case LED_0:
      for (int i = 1; i < 4; i++)
        bitClear(data2, i);
      break;
    }
    break;
  }
  digitalWrite(resultLatchPin, 0);
  shiftOut(resultDataPin, resultClockPin, data1);
  shiftOut(resultDataPin, resultClockPin, data2);
  digitalWrite(resultLatchPin, 1);
}

void printSingleDigit(char k)
{
  if (k != '*' && k != '#' && k != 'A' && k != 'B' && k != 'C' && k != 'D')
  {
    digitalWrite(digitLatchPin, 0);
    shiftOut(digitDataPin, digitClockPin, digitArray[whatchar(k)]);
    digitalWrite(digitLatchPin, 1);
    delay(500);
    digitClear();
  }
}

void printMultDigit(int text_len, char *text)
{
  for (int i = 0; i < text_len; i++)
  {
      printSingleDigit(text[i]);
      delay(250);
  }
}

char *print_color(int color)
{
  char *ret;
  switch (color)
  {
    case LED_R: ret = "red"; break;
    case LED_G: ret = "green"; break;
    case LED_Y: ret = "yellow"; break;
    case LED_0: ret = "off"; break;
  }
  return ret;
}

int whatchar(char k)
{
  int num = 0;
  switch (k)
  {
    case '0': num = 0; break;
    case '1': num = 1; break;
    case '2': num = 2; break;
    case '3': num = 3; break;
    case '4': num = 4; break;
    case '5': num = 5; break;
    case '6': num = 6; break;
    case '7': num = 7; break;
    case '8': num = 8; break;
    case '9': num = 9; break;
    case 'A': num = 10; break;
    case 'B': num = 11; break;
    case 'C': num = 12; break;
    case 'D': num = 13; break;
    case '*': num = 14; break;
    case '#': num = 15; break;
  }
  return num;
}

void digitClear()
{
  digitalWrite(digitLatchPin, 0);
  shiftOut(digitDataPin, digitClockPin, 0b00000000);
  digitalWrite(digitLatchPin, 1);
}

void printNumLeft()
{
  int disp_tries = 0;
  char ten[2];

  disp_tries = abs(max_tries - num_tries);
  ten[0] = (max_tries > 9 ? letters[disp_tries / 10] : '0');
  ten[1] = letters[disp_tries % 10];
  printMultDigit(2, ten);
}

// blinkall is never used
void blinkAll_2Bytes(int n, int d)
{
  digitalWrite(resultLatchPin, 0);
  shiftOut(resultDataPin, resultClockPin, 0);
  shiftOut(resultDataPin, resultClockPin, 0);
  digitalWrite(resultLatchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++)
  {
    digitalWrite(resultLatchPin, 0);
    shiftOut(resultDataPin, resultClockPin, 255);
    shiftOut(resultDataPin, resultClockPin, 255);
    digitalWrite(resultLatchPin, 1);
    delay(d);
    digitalWrite(resultLatchPin, 0);
    shiftOut(resultDataPin, resultClockPin, 0);
    shiftOut(resultDataPin, resultClockPin, 0);
    digitalWrite(resultLatchPin, 1);
    delay(d);
  }
}

// END OF CODE

