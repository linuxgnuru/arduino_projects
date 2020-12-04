#include <SPI.h>

const byte LATCH = 10;

int d1 = 3;
int d2 = 4;
int d3 = 5;
int d4 = 6;

byte data = 0b00000000;

// set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock

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

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
}

void loop()
{
  clearLeds();
  pickDigit(1);
  pickNumber((n / x / 1000) % 10, false);
  delayMicroseconds(del);
  clearLeds();
  pickDigit(2);
  pickNumber((n / x / 100) % 10, false);
  delayMicroseconds(del);
  clearLeds();
  pickDigit(3);
  pickNumber((n / x / 10) % 10, true);
  delayMicroseconds(del);
  clearLeds();
  pickDigit(4);
  pickNumber(n / x % 10, false);
  delayMicroseconds(del);
  n++;
//  if (digitalRead(7) == LOW) n = 0;
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


