// RadioShack POV Wand Arduino code
// by Amanda Ghassaei
// instructables.com/amandaghassaei
// April 2012

// for use with arduino uno or duemilanove with atmel328

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

// in most of this code I have used the arduino portpin assignments to send data to pins, you can read more about how that
// works here: http://www.arduino.cc/en/Reference/PortManipulation
// I've also included (and commented) the standard arduino library commands that perform the same functions and the port commands in case you are interested

#include <avr/pgmspace.h>//need to store letter arrays in flash memory- or else we run out of space, more info here: http://arduino.cc/en/Reference/PROGMEM

#include "letters.h"

/*******************************************************************************
  THIS NEXT SECTION IS WHAT YOU'LL WANT TO EDIT TO CREATE YOUR OWN MESSAGES
*******************************************************************************/
// setup
// PUT YOUR MESSAGE HERE!!- must be in all caps, spaces are fine, no punctuation
String povtext = "JOHN";
byte refreshrate = 1; // delay time for pixels to refresh in milliseconds- experiment with different values

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

// get length of string povtext
int dimtext = povtext.length();

const byte empty = 0b00000000;

//incoming data storage
byte data3 = 0; //for portB
byte data2 = 0; //for portC
byte data1 = 0; //for portD

//variables
byte n; //variable for loops
byte t; //variable for loops
byte l; //variable for loops

byte d[] = { 0b00000000, 0b00000000, 0b00000000 };

void digWrite(int pin, int val)
{
  if (pin > -1 && pin < 8)
  {
    if (val == HIGH) bitSet(d[0], pin);
    else bitClear(d[0], pin);
  }
  else if (pin > 7 && pin < 16)
  {
    if (val == HIGH) bitSet(d[1], abs(pin - 8));
    else bitClear(d[1], abs(pin - 8));
  }
  else if (pin > 15 && pin < 20)
  {
    if (val == HIGH) bitSet(d[2], abs(pin - 16));
    else bitClear(d[2], abs(pin - 16));
  }
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 3; i++)
    shiftOut(dataPin, clockPin, d[i]);
  digitalWrite(latchPin, 1);
}

// function to get array data
void sendToWand(const boolean letterArray[])
{
  int len;
  len = (letterArray == letterM || letterArray == letterW ? 15 : 12);
  // for each time step of matrix
  for (t = 0; t < len; t++)
  {
    // this loop sets digital pins 0-13 either high or low depending on the value of the corresponding matrix element
    for (int pin = 19; pin >= 0; pin--)
    {
      // if matrix element == 1 then turn led on
      if (letterArray[(13 - pin) * len + t])
        digWrite(pin, HIGH);
      // if matrix element == 0 then turn led off
      else
        digWrite(pin, LOW);
    }
    delay(refreshrate); // delay after each column of data is displayed
  } // go to next time step
}

void setup()
{
  // port/pin assignments- set all pins to output- more info here: http://www.arduino.cc/en/Reference/PortManipulation
  pinMode(latchPin, OUTPUT);
  pinMode(13, OUTPUT);
  // run intialization so we know device is working- leds should light up in order from top of wand to bottom
  // for each time step
  for (int pin = 19; pin >= 0; pin--)
  {
    digWrite(pin, HIGH);
    delay(100);
    digWrite(pin, LOW);
  }
  // clear ports- set all arduino pins to 0Volts
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 3; i++)
    shiftOut(dataPin, clockPin, empty);
  digitalWrite(latchPin, 1);
}

void loop()
{
  // space at beginning of text
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 3; i++)
    shiftOut(dataPin, clockPin, empty);
  digitalWrite(latchPin, 1);
  delay(refreshrate * 3);
  // go through each character of povtext and call function sendToWand to display letter
  for (n = 0; n < dimtext; n++)
  {
    if (povtext.charAt(n) == 'A') sendToWand(letterA);
    else if (povtext.charAt(n) == 'B') sendToWand(letterB);
    else if (povtext.charAt(n) == 'C') sendToWand(letterC);
    else if (povtext.charAt(n) == 'D') sendToWand(letterD);
    else if (povtext.charAt(n) == 'E') sendToWand(letterE);
    else if (povtext.charAt(n) == 'F') sendToWand(letterF);
    else if (povtext.charAt(n) == 'G') sendToWand(letterG);
    else if (povtext.charAt(n) == 'H') sendToWand(letterH);
    else if (povtext.charAt(n) == 'I') sendToWand(letterI);
    else if (povtext.charAt(n) == 'J') sendToWand(letterJ);
    else if (povtext.charAt(n) == 'K') sendToWand(letterK);
    else if (povtext.charAt(n) == 'L') sendToWand(letterL);
    else if (povtext.charAt(n) == 'M') sendToWand(letterM);
    else if (povtext.charAt(n) == 'N') sendToWand(letterN);
    else if (povtext.charAt(n) == 'O') sendToWand(letterO);
    else if (povtext.charAt(n) == 'P') sendToWand(letterP);
    else if (povtext.charAt(n) == 'Q') sendToWand(letterQ);
    else if (povtext.charAt(n) == 'R') sendToWand(letterR);
    else if (povtext.charAt(n) == 'S') sendToWand(letterS);
    else if (povtext.charAt(n) == 'T') sendToWand(letterT);
    else if (povtext.charAt(n) == 'U') sendToWand(letterU);
    else if (povtext.charAt(n) == 'V') sendToWand(letterV);
    else if (povtext.charAt(n) == 'W') sendToWand(letterW);
    else if (povtext.charAt(n) == 'X') sendToWand(letterX);
    else if (povtext.charAt(n) == 'Y') sendToWand(letterY);
    else if (povtext.charAt(n) == 'Z') sendToWand(letterZ);
    else if (povtext.charAt(n) == ' ')
    {
      digitalWrite(latchPin, 0);
      for (int i = 0; i < 3; i++)
        shiftOut(dataPin, clockPin, empty);
      digitalWrite(latchPin, 1);
      delay(refreshrate * 3); // off for 3 pixels
    }
    // space between each character
    digitalWrite(latchPin, 0);
    for (int i = 0; i < 3; i++)
      shiftOut(dataPin, clockPin, empty);
    digitalWrite(latchPin, 1);
    delay(refreshrate);
  }
  // space at end of text
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 3; i++)
    shiftOut(dataPin, clockPin, empty);
  digitalWrite(latchPin, 1);
  delay(refreshrate * 3);
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
    pinState = (myDataOut & (1 << i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

