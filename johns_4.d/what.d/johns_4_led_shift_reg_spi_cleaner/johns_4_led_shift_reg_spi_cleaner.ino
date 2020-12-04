#include <SPI.h>

const byte LATCH = 10;

#define DIG_NUM 4

int cols[DIG_NUM];

byte data = 0b00000000;

// set variable
long n = 20000; // 20000 = 20 seconds

int x = 100;
int del = 55;  //fine-tuning value for clock
int curLed = 1;

/*


TODO

add keypad support to set timmer.

*/


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
//  Serial.begin(9600);
  for (int i = 0; i < DIG_NUM; i++)
  {
    pinMode(i + 6, OUTPUT);
    cols[i] = i + 6;
  }
  pinMode(3, OUTPUT);
}

void loop()
{
  curLed = 1;
//  Serial.println(n);
  pickNumber(curLed++, (n / x / 1000) % 10, false);
  delayMicroseconds(del);
  pickNumber(curLed++, (n / x / 100) % 10, false);
  delayMicroseconds(del);
  pickNumber(curLed++, (n / x / 10) % 10, true);
  delayMicroseconds(del);
  pickNumber(curLed++, n / x % 10, false);
  delayMicroseconds(del);
//  if (n > 0)
    n--;
//  else    n++;
  //  delay(500);
  if (n < 0)
  {
    n = 20000;
    digitalWrite(3, HIGH);
  }
  //  if (digitalRead(7) == LOW) n = 0;
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


