#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
#define CEL 1
#define FAR 2

#define NUM 1

int cel_far_flag;

int dataPin  = 2; // Pin connected to DS of 74HC595 - blue wire
int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow wire

int digit1Sel = 5;
int digit2Sel = 6;
int digit3Sel = 7;

int alertPin = LED_BUILTIN;
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
  0b11011011  // 9
};

// for shift register leds:
int data;
int pos[] = { digit1Sel, digit2Sel, digit3Sel };

void setup()
{
//  randomSeed(analogRead(A4));
  Serial.begin(9600);
  pinMode(alertPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  for (int i = 0; i < 3; i++)
  {
    pinMode(pos[i], OUTPUT);
    digitalWrite(pos[i], HIGH);
  }
}

void loop()
{
  for (int j = 0; j < 3; j++)
  {
    pickCol(j);
    for (int i = 0; i < 10; i++)
    {
      printDigit(i);
      delay(500);
    }
  }
//  delay(1000);
}

void print3digit(int num, int slot, boolean dp_flag)
{
  data = digitArray[num];
  if (dp_flag == true)
    bitSet(data, 5);
  pickCol(slot);
}

void pickCol(int col)
{
  switch (col)
  {
    case 0:
      digitalWrite(pos[0], LOW);
      digitalWrite(pos[1], HIGH);
      digitalWrite(pos[2], HIGH);
      break;
    case 1:
      digitalWrite(pos[0], HIGH);
      digitalWrite(pos[1], LOW);
      digitalWrite(pos[2], HIGH);
      break;
    case 2:
      digitalWrite(pos[0], HIGH);
      digitalWrite(pos[1], HIGH);
      digitalWrite(pos[2], LOW);
      break;
    default:
      digitalWrite(pos[0], HIGH);
      digitalWrite(pos[1], HIGH);
      digitalWrite(pos[2], HIGH);
      break;
  }
}

void printDigit(int num)
{
  if (num > -1 && num < 10)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, digitArray[num]);
    digitalWrite(latchPin, 1);
  }
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
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

#if 0
void g2r(int d)
{
  int d1 = 0b00000000;
/*
  // loop through all printable digits for 7 segment LED
  for (int i = 0; i < MAX_DIG; i++)
  {
    digitalWrite(dLatchPin, 0);
    shiftOut(dDataPin, dClockPin, digitArray[i]);
    digitalWrite(dLatchPin, 1);
    delay(250);
  }
  */
  for (int i = 7; i > -1; i--)
  {
    bitSet(d1, i);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, d1);
    shiftOut(dataPin, clockPin, d2);
    digitalWrite(latchPin, 1);
    delay(d);
    if (i != 5 && i != 2)
    {
      bitClear(d1, i);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, d1);
      shiftOut(dataPin, clockPin, d2);
      digitalWrite(latchPin, 1);
    }
  }
  d1 = 0b00100100;
  for (int i = 7; i > 0; i--)
  {
    bitSet(d2, i);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, d1);
    shiftOut(dataPin, clockPin, d2);
    digitalWrite(latchPin, 1);
    delay(d);
    if (i != 7 && i != 4)
    {
      bitClear(d2, i);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, d1);
      shiftOut(dataPin, clockPin, d2);
      digitalWrite(latchPin, 1);
    }
  }
}
#endif

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

#if 0
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
    break;
  }
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data1);
  shiftOut(dataPin, clockPin, data2);
  digitalWrite(latchPin, 1);
}

void printSingleDigit(char k, int cf)
{
  if (k != '*' && k != '#' && k != 'A' && k != 'B' && k != 'C' && k != 'D')
  {
    digitalWrite(dLatchPin, 0);
    shiftOut(dDataPin, dClockPin, digitArray[k - 48]);
    digitalWrite(dLatchPin, 1);
    if (cf == true)
    {
      delay(500);
      shiftClear(dLatchPin, dDataPin, dClockPin, 1);
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

// possibly combine both clears; and have a if flag
void shiftClear(int latchP, int dataP, int clockP, int bNum)
{
  digitalWrite(latchP, 0);
  shiftOut(dataP, clockP, 0b00000000);
  if (bNum == 2)
    shiftOut(dataP, clockP, 0b00000000);
  digitalWrite(latchP, 1);
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

void printMessage(char *message, int sz)
{
  for (int i = 0; i < sz; i++)
  {
    digitalWrite(dLatchPin, 0);
    shiftOut(dDataPin, dClockPin, alphabetArray[message[i] - 65]);
    digitalWrite(dLatchPin, 1);
    delay(300);
    shiftClear(dLatchPin, dDataPin, dClockPin, 1);
    delay(100);
  }
}

void showWin()
{
  for (int j = 0; j < 6; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(dLatchPin, 0);
      shiftOut(dDataPin, dClockPin, winAnim[i]);
      digitalWrite(dLatchPin, 1);
      delay(90);
      shiftClear(dLatchPin, dDataPin, dClockPin, 1);
    }
  }
}
#endif
