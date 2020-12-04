/*
//set the anode interface
const int a = 1;
const int b = 2;
const int c = 3;
const int d = 4;
const int e = 5;
const int f = 6;
const int g = 7;
const int p = 8;
*/

#define DIG_NUM 3

const int pins[] = {
   5,  6,  7,  8, 9, 10, 11
};

const int dpPin = LED_BUILTIN;

// set the cathode interface
const int d[] = {
  2, 3, 4
};

//set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock
const byte inputPin = 12;

const boolean ledNum[][7] = {
//  a  b  c  d  e  f  g
//  e  d  c  g  a  f  b
  { 1, 1, 1, 0, 1, 1, 1 }, // 0
  { 0, 0, 1, 0, 0, 0, 1 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 0, 1, 1, 1, 1, 0, 1 }, // 3
  { 0, 0, 1, 1, 0, 1, 1 }, // 4
  { 0, 1, 1, 1, 1, 1, 0 }, // 5 b e
  { 1, 1, 1, 1, 1, 1, 0 }, // 6
  { 0, 0, 1, 0, 1, 0, 1 }, // 7
  { 1, 1, 1, 1, 1, 1, 1 }, // 8
  { 0, 1, 1, 1, 1, 1, 1 }  // 9
};

void setup()
{
  for (int i = 0; i < DIG_NUM; i++)
  {
    pinMode(d[i], OUTPUT);
    digitalWrite(d[i], HIGH);
  }
  for (int i = 0; i < 8; i++)
    pinMode(pins[i], OUTPUT);
  pinMode(inputPin, INPUT_PULLUP);
}

void loop()
{
  clearLEDs();
  pickDigit(0);
  pickNumber((n / x / 100) % 10);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(1);
  dispDec();
  pickNumber((n / x / 10) % 10);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(2);
  pickNumber(n / x % 10);
  delayMicroseconds(del);
  if ((++n > 999) || (digitalRead(inputPin) == LOW))
    n = 0;
}

void pickDigit(int x)  //define pickDigit(x),to open dx port
{
  for (int i = 0; i < DIG_NUM; i++)
    digitalWrite(d[i], HIGH);
  digitalWrite(d[x], LOW);
}

void pickNumber(int x)   //define pickNumber(x)to display number x
{
  for (int i = 0; i < 7; i++)
    digitalWrite(pins[i], (ledNum[x][i] == 1 ? HIGH : LOW));
}

void dispDec()  //set to start the decimal point
{
  digitalWrite(dpPin, HIGH);
}

void clearLEDs()  //clear contents
{
  for (int i = 0; i < 8; i++)
    digitalWrite(pins[i], LOW);
}

