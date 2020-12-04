// RadioShack POV Wand Arduino code
// by Amanda Ghassaei
// instructables.com/amandaghassaei
// April 2012

// for use with arduino uno or duemilanove with atmel328

/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
*/

// in most of this code I have used the arduino portpin assignments to send data to pins, you can read more about how that
// works here: http://www.arduino.cc/en/Reference/PortManipulation
// I've also included (and commented) the standard arduino library commands that perform the same functions and the port commands in case you are interested

#include <avr/pgmspace.h>//need to store letter arrays in flash memory- or else we run out of space, more info here: http://arduino.cc/en/Reference/PROGMEM
#include <SPI.h>

#include "letters.h"

/*******************************************************************************
  THIS NEXT SECTION IS WHAT YOU'LL WANT TO EDIT TO CREATE YOUR OWN MESSAGES
*******************************************************************************/
// setup
// PUT YOUR MESSAGE HERE!!- must be in all caps, spaces are fine, no punctuation
String povtext = "JOHN WIGGINS";
byte refreshrate = 1; // delay time for pixels to refresh in milliseconds- experiment with different values

const int LATCH = 10; // Pin connected to ST_CP of 74HC595 - green

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

#define MYDELAYA 75
#define MYDELAYB 75

void setup()
{
  for (int i = 2; i < 6; i++)
    pinMode(i, OUTPUT);
  //Serial.begin(9600);
  SPI.begin();
  // port/pin assignments- set all pins to output- more info here: http://www.arduino.cc/en/Reference/PortManipulation
  pinMode(LATCH, OUTPUT);
  // run intialization so we know device is working- leds should light up in order from top of wand to bottom
  // for each time step
  for (int pin = 19; pin > -1; pin--)
  {
    digWrite(pin, HIGH);
    delay(75);
    digWrite(pin, LOW);
//    delay(75);
  }
/*
  for (int i = 3; i > -1; i--)
  {
    for (int l = 0; l < 8; l++)
    {
      bitSet(d[i], l);
      digitalWrite(LATCH, LOW);
      SPI.transfer(d[0]);
      SPI.transfer(d[1]);
      SPI.transfer(d[2]);
      digitalWrite(LATCH, HIGH);
      delay(MYDELAYA);
    }
  }
  for (int i = 3; i > -1; i--)
  {
    for (int l = 0; l < 8; l++)
    {
      bitClear(d[i], l);
      digitalWrite(LATCH, LOW);
      SPI.transfer(d[0]);
      SPI.transfer(d[1]);
      SPI.transfer(d[2]);
      digitalWrite(LATCH, HIGH);
      if (l != 7)
        delay(MYDELAYB);
    }
  }
  */
  /*
    // clear ports- set all arduino pins to 0Volts
    digitalWrite(LATCH, LOW);
    for (int i = 0; i < 3; i++)
    SPI.transfer(empty);
    digitalWrite(LATCH, HIGH);
  */
  //  digWrite(19, HIGH);
  /*
    digitalWrite(LATCH, LOW);
    SPI.transfer(0b00000000);
    SPI.transfer(0b00000000);
    SPI.transfer(0b11100000);
    digitalWrite(LATCH, HIGH);
  */
}

void loop()
{
  /*
  bitWrite(fu, 5, HIGH);
  digWrite(3, HIGH);
  digWrite(2, HIGH);
  digWrite(1, HIGH);
  digWrite(0, HIGH);
  byte fu = empty;
  bitWrite(fu, 7, HIGH);
  bitWrite(fu, 6, HIGH);
  bitWrite(fu, 5, HIGH);
  bitWrite(fu, 4, HIGH);
  digitalWrite(LATCH, LOW);
  SPI.transfer(empty);
  SPI.transfer(empty);
  SPI.transfer(fu);
  digitalWrite(LATCH, HIGH);
*/
#if 0
  d[2] = d[0] = empty;
  bitWrite(d[1], 7, HIGH);
  digitalWrite(LATCH, LOW);
  SPI.transfer(d[0]);
  SPI.transfer(d[1]);
  SPI.transfer(d[2]);
  digitalWrite(LATCH, HIGH);
#else
  // space at beginning of text
  clsAll();
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
      clsAll();
      delay(refreshrate * 3); // off for 3 pixels
    }
    // space between each character
    clsAll();
    delay(refreshrate);
  }
  // space at end of text
  clsAll();
  delay(refreshrate * 3);
  #endif
}

void clsAll()
{
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < 3; i++)
    SPI.transfer(empty);
  digitalWrite(LATCH, HIGH);

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
      digWrite(pin, letterArray[(19 - pin) * len + t]);
    }
    delay(refreshrate); // delay after each column of data is displayed
  } // go to next time step
}

void digWrite(int pin, int val)
{
  // Do nothing if pin is invalid
  if (pin < 0 || pin > 19)
    return;
    /*
  Serial.print("abs pin - 8: ");
  Serial.print(abs(pin - 8));
  Serial.print(" abs pin - 16: ");
  Serial.println(abs(pin - 16));
  */
  if (pin > 11 && pin < 20)
    bitWrite(d[0], abs(pin - 12), val);
  else if (pin > 3 && pin < 12)
    bitWrite(d[1], abs(pin - 4), val);
  else if (pin > -1 && pin < 4)
  {
    /*
    switch (abs(pin - 4))
    {
      case 
    }
    */
    bitWrite(d[2], pin + 4, val);
  }
  digitalWrite(LATCH, LOW);
  SPI.transfer(d[0]);
  SPI.transfer(d[1]);
  SPI.transfer(d[2]);
  digitalWrite(LATCH, HIGH);
}

