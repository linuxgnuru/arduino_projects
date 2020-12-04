/*
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

*/
#include <SPI.h>
#include <Keypad.h>


const byte LATCH = 10;

#define DIG_NUM 4
#define MAX_NUM 20000

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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const byte ledNum[] = {
  //abcdefg
  0b11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110  // 9
};
int cols[DIG_NUM];

byte data = 0b00000000;

boolean displayFlag = false;

// set variable
long n = MAX_NUM; // 20000 = 20 seconds

int x = 100;
int del = 55;  //fine-tuning value for clock
int curLed = 1;
long num = 0;

boolean startFlag = false;
boolean dFlag = false;
char c;
int inp;
int curCol = 0;
long inpCols[4];
int inpCtr = 0;

long dig1;
long dig2;
long dig3;
long dig4;

void setup()
{
  SPI.begin();
  //  Serial.begin(9600);
  int j = 0;
  for (int i = A0; i < A4; i++)
  {
    pinMode(i, OUTPUT);
    cols[j++] = i;
  }
  pinMode(12, OUTPUT); // for buzzer
}

void loop()
{
  char key = keypad.getKey();

  num = 0;
  curLed = 1;
  if (key != NO_KEY)
  {
    c = key;
    switch (c)
    {
      case '*': startFlag = true; break;
      case '#':
        startFlag = false;
        dFlag = false;
        curLed = 1;
        n = MAX_NUM;
        inpCtr = 0;
        num = 0;
        curCol = 0;
        dig1 = dig2 = dig3 = dig4 = 0;
        displayFlag = false;
        for (int i = 0; i < 4; i++)
          inpCols[i] = 0;
        break;
      /*
        20---
        20000 = 20 seconds
        60000 = 60 seconds - 1 minute
        90000 = 90 seconds - 1.5 minutes
       120000 = 120 seconds - 2 minutes
       9999
      */
      case 'A': // 30 seconds
        inpCols[3] = 0;
        inpCols[2] = 0;
        inpCols[1] = 3;
        inpCols[0] = 0;
        inpCtr = 3;
        displayFlag = true;
        startFlag = true;
        break;
      case 'B': // 1 minute
        inpCols[3] = 0;
        inpCols[2] = 0;
        inpCols[1] = 6;
        inpCols[0] = 0;
        inpCtr = 3;
        displayFlag = true;
        startFlag = true;
        break;
      case 'C': // 2 minutes
        inpCols[3] = 0;
        inpCols[2] = 0;
        inpCols[1] = 2;
        inpCols[0] = 1;
        inpCtr = 3;
        displayFlag = true;
        startFlag = true;
        break;
      case 'D': // 5 minutes
        inpCols[3] = 0;
        inpCols[2] = 0;
        inpCols[1] = 0;
        inpCols[0] = 3;
        inpCtr = 3;
        displayFlag = true;
        startFlag = true;
        break;
      case '1': case '2': case '3': case '4': case '5':
      case '6': case '7': case '8': case '9': case '0':
        inp = c - 48;
        pickNumber(4, inp, false);
        displayFlag = true;
        inpCols[curCol++] = c - 48;
        inpCtr++;
        break;
      default: break;
    }
//    dig1 = inpCols[3] * 100;
    dig1 = 0;
    dig2 = inpCols[2] * 1000;
    dig3 = inpCols[1] * 10000;
    dig4 = inpCols[0] * 100000;
    num = dig4 + dig3 + dig2 + dig1;
    n = num;
  }
  if (inpCtr >= 3 && dFlag == false)
  {
    pickNumber(4, 0, true);
    dFlag = true;
  }
  if (startFlag == true)
  {
    if (inpCtr == 3)
    {
      pickNumber(curLed++, (n / x / 1000) % 10, false);
      delayMicroseconds(del);
      pickNumber(curLed++, (n / x / 100) % 10, false);
      delayMicroseconds(del);
      pickNumber(curLed++, (n / x / 10) % 10, true);
      delayMicroseconds(del);
      pickNumber(curLed++, n / x % 10, false);
      delayMicroseconds(del);
      n--;
      if (n < 0)
      {
        //n = num;
        
      for (int i = A0; i < A4; i++)
        digitalWrite(i, HIGH);
        delay(200);
      for (int i = A0; i < A4; i++)
        digitalWrite(i, LOW);
        delay(200);
        digitalWrite(12, HIGH);
      }
    }
  }
  else
  {
    if (displayFlag == false)
    {
      for (int i = A0; i < A4; i++)
        digitalWrite(i, HIGH);
    }
  }
}

void pickNumber(int pos, int x, boolean dp)   //define pickNumber(x)to display number x
{
  if (x >= 0)
  {
    // first, lets get the right number
    data = ledNum[x];

    // next, check to see if the decimal point should be on or not
    if (dp == true) bitSet(data, 0);
    else bitClear(data, 0);

    // next, clear everything
    digitalWrite(LATCH, LOW);
    SPI.transfer(0b00000000);
    digitalWrite(LATCH, HIGH);

    // next pick the digit column
    for (int i = 0; i < DIG_NUM; i++)
      digitalWrite(cols[i], HIGH);
    digitalWrite(cols[pos - 1], LOW);

    // Now we turn on the right leds
    digitalWrite(LATCH, LOW);
    SPI.transfer(data);
    digitalWrite(LATCH, HIGH);
  }
}

