int ff = false;
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
#define NUM_LEDS 5
#define MAX_DIG 10 // 10 = 0-9, 14 = 0-9, A-D

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

int alertPin = LED_BUILTIN;

/*
 * Keypad code:
 */
const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = { 2, 3, 4, 5 }; // connect to row pinouts 
byte colPins[COLS] = { 6, 7, 8, 9 }; // connect to column pinouts

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

/*
 --    H
|  | A   G
 --    B
|  | C   E
 -- .  D   F (.)
*/
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

byte alphabetArray[] = {
  0b11010111, // A
  0b11011111, // B
  0b10001101, // C
  0b11111101, // D
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
  0b00000000, // Q XX NOTHING
  0b00000110, // R (r)
  0b10011011, // S
  0b11010000, // T
  0b01011101, // U
  0b00011100, // V
  0b00000000, // W XX NOTHING
  0b00000000, // X XX NOTHING
  0b01011011, // Y
  0b00000000  // Z XX NOTHING
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
  0b11011011  // 9
};

/*
  lengths:
  10 = 0-9
  14 = 0-9 + A-D
  16 = 0-9 + A-D + * + #
 */
/*
  0b11011011, // 9
  0b11110111, // A
  0b11111111, // B
  0b10101101, // C
  0b11111101, // D
  0b01110111, // *
  0b10110110  // #
};
*/
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

/*
  Might not need the following; just do char - 48
  e.g.
  char i = '7';
  [i - 48] = int 7
  */
const char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D' };

long winPreviousMillis = 0;
long losePreviousMillis = 0;

long interval = 300;

// Flags
int resetFlag = false;
int firstFlag = true;
int winFlag = false;
int loseFlag = false;
int winSwapFlag = false;
int loseSwapFlag = false;
int shuffleFlag = true;

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

// counters
int correct_index = 0;
int correct_counter = 0;
int num_tries = 0;

void setup()
{
  Serial.begin(9600);
  firstFlag = true;
  pinMode(alertPin, OUTPUT);
  digitalWrite(alertPin, LOW);
  pinMode(resultLatchPin, OUTPUT);
  pinMode(digitLatchPin, OUTPUT);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    answer[i] = 'E';
    result[i] = LED_0;
  }
  for (int i = 0; i < MAX_DIG; i++)
    used[i] = 0;
//  printHello();
  printMessage("HELLOQJOHNQPLAY", 15);
//  printMessage("LETS", 3);
  create_passcode();
  digitClear();
  resultClear();
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY && (key != '*' && key != '#'))
  {
    digitClear();
    printSingleDigit(key, false);
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
        resultClear();
      }
      if (key == '*')
      {
        shuffleFlag = true;
        ff = false;
        printNumLeft();
        blip(2, 100);
      }
    }
    if (key == '#')
    {
//      printMessage("RESET", 5);
      correct_index = correct_counter = 0;
      resultClear();
      digitClear();
      blip(3, 100);
      winFlag = false;
      loseFlag = false;
      resetFlag = true;
      num_tries = 0;
      for (int i = 0; i < NUM_LEDS; i++)
        result[i] = LED_0;
      for (int i = 0; i < MAX_DIG; i++)
        used[i] = 0;
      create_passcode();
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
        printMessage("PASS", 4);
        delay(200);
        printMultDigit(passcode, 5);
        break;
      case 'C':
        ff = false;
        showWin();
        break;
      // display demo
      case 'D':
        g2r(150);
        digitClear();
        resultClear();
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
      answer[correct_index++] = key;
    }
    // we've gotten all the digits we need
    if (correct_index == NUM_LEDS && winFlag == false)
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
      }
      // Test to see if you lost
      if (num_tries > max_tries)
      {
        loseFlag = true;
        winFlag = false;
      }
    }
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
      }
      else
      {
        data1 = 0b01001001;
        data2 = 0b00100100;
        digitalWrite(resultLatchPin, 0);
        shiftOut(resultDataPin, resultClockPin, data1);
        shiftOut(resultDataPin, resultClockPin, data2);
        digitalWrite(resultLatchPin, 1);
      }
      if (loseCurrentMillis - losePreviousMillis > interval)
      {
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
      }
      else
      {
        data1 = 0b01001001;
        data2 = 0b00100100;
        digitalWrite(resultLatchPin, 0);
        shiftOut(resultDataPin, resultClockPin, data1);
        shiftOut(resultDataPin, resultClockPin, data2);
        digitalWrite(resultLatchPin, 1);
      }
      if (winCurrentMillis - winPreviousMillis > interval)
      {
        winPreviousMillis = winCurrentMillis;
        winSwapFlag = (winSwapFlag == false ? true : false);
      }
    }
    if (winFlag == false && loseFlag == false)
    {
      if (result[NUM_LEDS-1] != LED_0)
      {
        if (ff == false)
        {
          Serial.print("before: [");
          for (int j = 0; j < NUM_LEDS; j++)
            Serial.print(result[j]);
          Serial.println("]");
          shuffle_result();
          Serial.print(" after: [");
          for (int j = 0; j < NUM_LEDS; j++)
            Serial.print(result[j]);
          Serial.println("]");
          ff = true;
        }
        else
        {
          // Shuffle the result
          if (shuffleFlag == true)
          {
            shuffle_result();
            for (int i = 0; i < NUM_LEDS; i++)
              changeBit(i, result[i]);
            shuffleFlag = false;
          }
        }
      }
    }
  }
  delay(100);
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void shuffle_result()
{
  int ran;
  int i;

  randomSeed(analogRead(A0));
  for (i = NUM_LEDS - 1; i > 0; i--)
  {
    ran = random(0, i);
    swap(&result[i], &result[ran]);
  }
}

void create_passcode()
{
  int ran;
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
  // loop through all printable digits for 7 segment LED
  for (int i = 0; i < MAX_DIG; i++)
  {
    digitalWrite(digitLatchPin, 0);
    shiftOut(digitDataPin, digitClockPin, digitArray[i]);
    digitalWrite(digitLatchPin, 1);
    delay(250);
  }
  for (int i = 7; i > -1; i--)
  {
    bitSet(d1, i);
    digitalWrite(resultLatchPin, 0);
    shiftOut(resultDataPin, resultClockPin, d1);
    shiftOut(resultDataPin, resultClockPin, d2);
    digitalWrite(resultLatchPin, 1);
    delay(d);
    if (i != 5 && i != 2)
    {
      bitClear(d1, i);
      digitalWrite(resultLatchPin, 0);
      shiftOut(resultDataPin, resultClockPin, d1);
      shiftOut(resultDataPin, resultClockPin, d2);
      digitalWrite(resultLatchPin, 1);
    }
  }
  d1 = 0b00100100;
  for (int i = 7; i > 0; i--)
  {
    bitSet(d2, i);
    digitalWrite(resultLatchPin, 0);
    shiftOut(resultDataPin, resultClockPin, d1);
    shiftOut(resultDataPin, resultClockPin, d2);
    digitalWrite(resultLatchPin, 1);
    delay(d);
    if (i != 7 && i != 4 && i != 1)
    {
      bitClear(d2, i);
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
    digitalWrite(alertPin, HIGH);
    delay(d);
    digitalWrite(alertPin, LOW);
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

int whatchar(char k)
{
  return (k - 48);
#if 0
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
#endif
}

void printSingleDigit(char k, int cf)
{
  if (k != '*' && k != '#' && k != 'A' && k != 'B' && k != 'C' && k != 'D')
  {
    digitalWrite(digitLatchPin, 0);
    shiftOut(digitDataPin, digitClockPin, digitArray[whatchar(k)]);
    digitalWrite(digitLatchPin, 1);
    if (cf == true)
    {
      delay(500);
      digitClear();
    }
  }
}

void printMultDigit(char *text, int text_len)
{
  for (int i = 0; i < text_len; i++)
  {
      printSingleDigit(text[i], true);
      delay(100);
  }
}

void digitClear()
{
  digitalWrite(digitLatchPin, 0);
  shiftOut(digitDataPin, digitClockPin, 0b00000000);
  digitalWrite(digitLatchPin, 1);
}

void resultClear()
{
  digitalWrite(resultLatchPin, 0);
  shiftOut(resultDataPin, resultClockPin, 0b00000000);
  shiftOut(resultDataPin, resultClockPin, 0b00000000);
  digitalWrite(resultLatchPin, 1);
}

void printNumLeft()
{
  int disp_tries = 0;
  char ten[2];

  disp_tries = abs(max_tries - num_tries);
  ten[0] = (max_tries > 9 ? letters[disp_tries / 10] : '0');
  ten[1] = letters[disp_tries % 10];
  printMultDigit(ten, 2);
}

void printMessage(char *message, int sz)
{
  for (int i = 0; i < sz; i++)
  {
    digitalWrite(digitLatchPin, 0);
    shiftOut(digitDataPin, digitClockPin, alphabetArray[message[i] - 65]);
    digitalWrite(digitLatchPin, 1);
    delay(300);
    digitClear();
    delay(100);
  }
}

void showWin()
{
  for (int j = 0; j < 6; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(digitLatchPin, 0);
      shiftOut(digitDataPin, digitClockPin, winAnim[i]);
      digitalWrite(digitLatchPin, 1);
      delay(90);
      digitClear();
    }
  }
}

