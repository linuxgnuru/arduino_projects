#include <SPI.h>

const byte LATCH = 10;
byte data = 0b00000000;

// set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock

const byte ledNum[] = {
  //ed.cgbaf
  //gf.edcba
  0b11010111, // 0
  //ed.cgbaf
  //gf.edcba
  0b00010100, // 1
  //ed.cgbaf
  //gf.edcba
  0b11001110, // 2 f c
  //ed.cgbaf
  //gf.edcba
  0b01011110, // 3 f e
  //ed.cgbaf
  //gf.edcba
  0b00010111, // 4 g e d
  //ed.cgbaf
  //gf.edcba
  0b01011011, // 5 e b
  //ed.cgbaf
  //gf.edcba
  0b11011011, // 6 b
  //ed.cgbaf
  //gf.edcba
  0b10001001, // 7 g f e
  0b11011111, // 8
  //ed.cgbaf
  //gf.edcba
  0b01011111  // 9 e
};

int d4 = 4;
int d3 = 5;
int d2 = 6;
int d1 = 7;

void setup()
{
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  SPI.begin();
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
}

void loop()
{

  clearLeds();
  pickDigit(4);
  pickNumber((n / x / 1000) % 10, false);
    delayMicroseconds(del);
  clearLeds();
  pickDigit(3);
  pickNumber((n / x / 100) % 10, false);
    delayMicroseconds(del);
  clearLeds();
  pickDigit(2);
  pickNumber((n / x / 10) % 10, true);
    delayMicroseconds(del);
  clearLeds();
  pickDigit(1);
  pickNumber(n / x % 10, false);
    delayMicroseconds(del);
  n++;
  //  if (digitalRead(13) == LOW) n = 0;
}

void pickDigit(int x)  //define pickDigit(x),to open dx port
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  switch (x)
  {
    case 1:
      digitalWrite(d1, LOW);
      break;
    case 2:
      digitalWrite(d2, LOW);
      break;
    case 3:
      digitalWrite(d3, LOW);
      break;
    default:
      digitalWrite(d4, LOW);
      break;
  }
}

void clearLeds()
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00000000);
  digitalWrite(LATCH, HIGH);
}

void pickNumber(int x, boolean dp)   //define pickNumber(x)to display number x
{
  data = ledNum[x];
  if (dp == true) bitSet(data, 0);
  else bitClear(data, 0);
  digitalWrite(LATCH, LOW);
  SPI.transfer(data);
  digitalWrite(LATCH, HIGH);
}

