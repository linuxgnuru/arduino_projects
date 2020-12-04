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

int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

int digit1 = 11; // PWM Display pin 1
int digit2 = 10; // PWM Display pin 2
int digit3 =  9; // PWM Display pin 6

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

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
//  blip(3, 100);
//  blinkAll_2Bytes(4, 250);
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
}

void loop()
{
  digitalWrite(digit1, LOW);
  digitalWrite(latchPin, 0); // 01234567
  shiftOut(dataPin, clockPin, 0b00000011);
  digitalWrite(latchPin, 1);
  delayMicroseconds(500);
  digitalWrite(digit1, HIGH);
//  zero2nine();
//  delay(500);
  digitalWrite(digit1, LOW);
  digitalWrite(latchPin, 0); // 01234567
  shiftOut(dataPin, clockPin, 0b00000000);
  digitalWrite(latchPin, 1);
  digitalWrite(digit1, HIGH);
  delayMicroseconds(500);
  
//  green2red(400);
//  delay(500);
}

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
    case 0:
      bitSet(dataResult, (pos * 2));
      bitClear(dataResult, (pos * 2) + 1);
      break;
    case 1:
      bitClear(dataResult, (pos * 2));
      bitSet(dataResult, (pos * 2) + 1);
      break;
    case 2:
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
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (int i = 7; i >= 0; i--)
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
  int d1 = 0;
  int d2 = 0;
  int Lights[] = { 0, 1 };

  dataResult = 0;
  for (int i = 3; i > -1; i--)
  {
    for (int j = 0; j < 2; j++)
    {
      changeBit(i, Lights[j]);
      delay(del);
    }
  }
  shiftClear(0);
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

// possibly combine both clears; and have a if flag
void shiftClear(int dr)
{
  byte d1 = 0;
  byte d2 = 0;

  digitalWrite(latchPin, 0);
  if (dr == 0)
    d1 = dataDigit;
  if (dr == 1)
    d2 = dataResult;
  shiftOut(dataPin, clockPin, d1);
  shiftOut(dataPin, clockPin, d2);
  digitalWrite(latchPin, 1);
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
      shiftClear(1);
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
    shiftClear(1);
    delay(100);
  }
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
    shiftClear(1);
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
  byte Digit = (num > 9 ? 0 : digitArray[num]);

  // print decimal
  //     543210
  // 0b00100000
  if (decimalFlag == true)
    bitSet(Digit, 5);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, Digit);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
}
#if 0
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
#endif

