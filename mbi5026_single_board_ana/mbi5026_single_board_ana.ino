#include <SPI.h>

#include "enums.h"
#include "pins.h"
#include "data.h"

#define MIN_ 500
#define MAX_ 1020
#define MX 24

// delays
#define DL 50
#define RDL 100

unsigned long lastMillis = 0;

void setup()
{
  SPI.begin();
  pinMode(SPI_LATCH, OUTPUT);
  writeData(BOARD_SPI);
  //Serial.begin(9600);
  for (int i = 0; i < 2; i++)
  {
    pinMode(dPins[i][DATA], OUTPUT);
    pinMode(dPins[i][LATCH], OUTPUT);
    pinMode(dPins[i][CLOCK], OUTPUT);
  }
  writeData(BOARD_SPI);
  writeData(BOARD_B);
  //data[BOARD_1][0] = data[BOARD_1][1] = data[BOARD_1][2] =  data[BOARD_1][3] = 0b11111000;  writeData(BOARD_1);
}

void loop()
{
  //int v = map(analogRead(anaPin), MIN_, MAX_, 0, 6);
  //parseData(v, LEFT); // LEFT = 0, RIGHT = 1
  //led(lednum, LED_ON, BOARD_0);
  doAna(A1);
}

void led(int lednum, int cmd)
{
  if ((lednum < 0 || lednum > 15) || (cmd < LED_ON || cmd > LED_A_OFF))
    return;
  switch (cmd)
  {
    case LED_ON:
      if (lednum < 8)
      {
        bitSet(data[0][0], lednum);
        bitSet(data[0][2], lednum);
        bitSet(data[1][0], lednum);
        bitSet(data[1][2], lednum);
      }
      else
      {
        lednum = abs(lednum - 8);
        bitSet(data[0][1], lednum);
        bitSet(data[0][3], lednum);
        bitSet(data[1][1], lednum);
        bitSet(data[1][3], lednum);
      }
      break;
    case LED_OFF:
      if (lednum < 8)
      {
        bitClear(data[0][0], lednum);
        bitClear(data[0][2], lednum);
        bitClear(data[1][0], lednum);
        bitClear(data[1][2], lednum);
      }
      else
      {
        lednum = abs(lednum - 8);
        bitClear(data[0][1], lednum);
        bitClear(data[0][3], lednum);
        bitClear(data[1][1], lednum);
        bitClear(data[1][3], lednum);
      }
      break;
    case LED_TOGGLE:
      if (lednum < 8)
      {
        bitWrite(data[0][0], lednum, !bitRead(data[0][0], lednum));
        bitWrite(data[0][2], lednum, !bitRead(data[0][2], lednum));
        bitWrite(data[1][0], lednum, !bitRead(data[1][0], lednum));
        bitWrite(data[1][2], lednum, !bitRead(data[1][2], lednum));
      }
      else
      {
        lednum = abs(lednum - 8);
        bitWrite(data[0][1], lednum, !bitRead(data[0][1], lednum));
        bitWrite(data[0][3], lednum, !bitRead(data[0][3], lednum));
        bitWrite(data[1][1], lednum, !bitRead(data[1][1], lednum));
        bitWrite(data[1][3], lednum, !bitRead(data[1][3], lednum));
      }
      break;
    case LED_LE_A_ON: // led_left_all_on
      for (int i = 3; i < 8; i++)
      {
        bitSet(data[0][0], i);
        bitSet(data[0][2], i);
        bitSet(data[1][0], i);
        bitSet(data[1][2], i);
      }
      break;
    case LED_RI_A_ON:
      for (int i = 11; i < 16; i++)
      {
        bitSet(data[0][1], i);
        bitSet(data[0][3], i);
        bitSet(data[1][1], i);
        bitSet(data[1][3], i);
      }
      break;
    case LED_LE_A_OFF:
      for (int i = 3; i < 8; i++)
      {
        bitClear(data[0][0], i);
        bitClear(data[0][2], i);
        bitClear(data[1][0], i);
        bitClear(data[1][2], i);
      }
      break;
    case LED_RI_A_OFF:
      for (int i = 11; i < 16; i++)
      {
        bitClear(data[0][1], i);
        bitClear(data[0][3], i);
        bitClear(data[1][1], i);
        bitClear(data[1][3], i);
      }
      break;
    case LED_A_ON:
      for (int i = 0; i < 4; i++)
        data[0][i] = data[1][i] = 0b11111111;
      break;
    case LED_A_OFF:
      for (int i = 0; i < 4; i++)
        data[0][i] = data[1][i] = 0b00000000;
      break;
    default:
      break;
  }
  writeData();
}

void writeData()
{
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(dPins[i][LATCH], LOW);
    shiftOut(dPins[i][DATA], dPins[i][CLOCK], MSBFIRST, data[i][0]);
    shiftOut(dPins[i][DATA], dPins[i][CLOCK], LSBFIRST, data[i][1]);
    shiftOut(dPins[i][DATA], dPins[i][CLOCK], MSBFIRST, data[i][2]);
    shiftOut(dPins[i][DATA], dPins[i][CLOCK], LSBFIRST, data[i][3]);
    digitalWrite(dPins[i][LATCH], HIGH);
  }
}

void RunDemo()
{
  for (int i = 7; i > 2; i--)
  {
    led(i, LED_ON);
    delay(DL);
  }
  for (int i = 11; i < 16; i++)
  {
    led(i, LED_ON);
    delay(DL);
  }
  for (int i = 0; i < 3; i++)
  {
    led(l_r[0][i], LED_ON);
    led(l_r[0][i], LED_ON);
    delay(DL);
    led(l_r[1][i], LED_ON);
    led(l_r[1][i], LED_ON);
    delay(DL);
  }
  for (int i = 3; i < 8; i++)
  {
    led(i, LED_OFF);
    delay(DL);
  }
  for (int i = 15; i > 10; i--)
  {
    led(i, LED_OFF);
    delay(DL);
  }
  for (int i = 2; i > -1; i--)
  {
    led(l_r[0][i], LED_OFF);
    led(l_r[0][i], LED_OFF);
    delay(DL);
    led(l_r[1][i], LED_OFF);
    led(l_r[1][i], LED_OFF);
    delay(DL);
  }
}

void Blink()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

// v = a number between 0 and 5
// 0 all leds off, 5 all leds on
// so really only mess with data if it's between 1 and 4
void parseData(int v, boolean l_r)
{
  int d_a, d_b;
  int myV;
  //void led(int lednum, int cmd)

  // set lednum to anything; it won't matter
  if (v == 0 || v == 5)
  {
    if (v == 0)
      led(0, (!l_r ? LED_LE_A_OFF : LED_RI_A_OFF));
    else
      led(0, (!l_r ? LED_LE_A_ON : LED_RI_A_ON));
  }
  else
  {
    myV = (!l_r ? abs(v - 7) : v + 11);
    /*
       myV will be
       the range will be 1 to 5; if 0, clear all leds
       if 5, set all leds
       if > 0 || < 5 continue
      if left side (l_r == 0); if v == 2 then clear 3 and 4
      if right side (l_r == 1); if v == 2 then clear 14 and 15

      0 will just have all leds off

       left side        right side
       p  3 v = 5       p 15 v = 5
       p  4 v = 4       p 14 v = 4
       p  5 v = 3       p 13 v = 3
       p  6 v = 2       p 12 v = 2
       p  7 v = 1       p 11 v = 1

       or

       v = 0 - 15
    */
    // v will be between 1 and 3 now
    // first clear the leds
    for (int i = v; i < 5; i++)
    {

    }
  }
}

#if 0
void doAna(byte ap)
{
  int pin, chip;
  //int v = map(analogRead(anaPins[p]), MIN_, MAX_, 0, MX);
  int v = map(analogRead(ap), MIN_, MAX_, 0, MX);
  int nv;

  for (int i = v; i < MX; i++)
  {
    pin = i % 8;
    chip = abs(i / 8 - 2);
    bitClear(SPI_d[chip], pin);
  }
  if (v != 0)
  {
    if (v == MX - 1)
      nv = v + 1;
    else
      nv = v;
    for (int i = 0; i < nv; i++)
    {
      pin = i % 8;
      chip = abs(i / 8 - 2);
      bitSet(SPI_d[chip], pin);
    }
  }
  writeData();
}
#endif

