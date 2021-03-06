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

#include "wand.h"

/*******************************************************************************
  THIS NEXT SECTION IS WHAT YOU'LL WANT TO EDIT TO CREATE YOUR OWN MESSAGES
*******************************************************************************/
// setup

// PUT YOUR MESSAGE HERE!!- must be in all caps, spaces are fine, no punctuation
String povtext = "HELLO MOM AND DAD";
byte refreshrate = 1; //delay time for pixels to refresh in milliseconds- experiment with different values

// get length of string povtext
int dimtext = povtext.length();

//incoming data storage
byte data1 = 0; //for portB
byte data2 = 0; //for portC
byte data3 = 0; //for portD

//variables
byte n; //variable for loops
byte t; //variable for loops
byte l; //variable for loops

// function to get array data
void sendToWand(const boolean letterArray[])
{
  int len;
  len = (letterArray == letterM || letterArray == letterW ? 15 : 12);
  // for each time step
  for (t = 0; t < len; t++)
  {
    // for first six rows of data
    for (l = 0; l < 6; l++)
    {
      data1 = data1 << 1; // bitwise shift left
      data1 |= pgm_read_byte_near(letterArray + (l * len + t)); // add next value from dataset
    }
    // for next eight rows of data
    for (l = 6; l < 14; l++)
    {
      data2 = data2 << 1; // bitwise shift left
      data2 |= pgm_read_byte_near(letterArray + (l * len + t)); // add next value from dataset
    }
    // for next six rows of data
    for (l = 14; l < 20; l++)
    {
      data3 = data3 << 1; // bitwise shift left
      data3 |= pgm_read_byte_near(letterArray + (l * len + t)); // add next value from dataset
    }
    // SET PINS:
    PORTB = data1;
    PORTD = data2;
    PORTC = data3;
    delay(refreshrate);
    // clear data storage
    data1 = 0;
    data2 = 0;
    data3 = 0;
  }
}

// M and W are extra wide- they have a special function to get array data (with 15 time steps instead of 12)
void sendToWandMW(const boolean letterArray[])
{
  // send data to leds via port/pin manipulation
  // for each time step
  for (t = 0; t < 15; t++)
  {
    // for first six rows of data
    for (l = 0; l < 6; l++)
    {
      data1 = data1 << 1; // bitwise shift left
      data1 |= pgm_read_byte_near(letterArray + (l * 15 + t)); // add next value from dataset
    }
    // for next eight rows of data
    for (l = 6; l < 14; l++)
    {
      data2 = data2 << 1; // bitwise shift left
      data2 |= pgm_read_byte_near(letterArray + (l * 15 + t)); // add next value from dataset
    }
    // for next six rows of data
    for (l = 14; l < 20; l++)
    {
      data3 = data3 << 1; // bitwise shift left
      data3 |= pgm_read_byte_near(letterArray + (l * 15 + t)); // add next value from dataset
    }
    // SET PINS:
    PORTB = data1;
    PORTD = data2;
    PORTC = data3;
    delay(refreshrate);
    // clear data storage
    data1 = 0;
    data2 = 0;
    data3 = 0;
  }
}

#if 0
void dosomething()
{
  // SAME AS ABOVE BUT WRITTEN USING ARDUINO LIBRARY COMMANDS:
  // for each time step of matrix
  for (t = 0; t < 12; t++)
  {
    // this loop sets digital pins 0-13 either high or low depending on the value of the corresponding matrix element
    for (int pin = 13; pin >= 0; pin--)
    {
      // if matrix element == 1 then turn led on
      if (letterArray[(13 - pin) * 12 + t])
        digitalWrite(pin, HIGH);
      // if matrix element == 0 then turn led off
      else
        digitalWrite(pin, LOW);
    }
    // the following lines address the analog pins indivially and set them either high or low depending on matrix element
    // if matrix element == 1 then turn led on
    if (letterArray[14 * 12 + t])
      digitalWrite(A5, HIGH);
    // if matrix element == 0 then turn led off
    else
      digitalWrite(A5, LOW);
    if (letterArray[15 * 12 + t])
      digitalWrite(A4, HIGH);
    else
      digitalWrite(A4, LOW);
    if (letterArray[16 * 12 + t])
      digitalWrite(A3, HIGH);
    else
      digitalWrite(A3, LOW);
    if (letterArray[17 * 12 + t])
      digitalWrite(A2, HIGH);
    else
      digitalWrite(A2, LOW);
    if (letterArray[18 * 12 + t])
      digitalWrite(A1, HIGH);
    else
      digitalWrite(A1, LOW);
    if (letterArray[19 * 12 + t])
      digitalWrite(A0, HIGH);
    else
      digitalWrite(A0, LOW);
    delay(refreshrate); // delay after each column of data is displayed
  } // go to next time step
}
#endif

void setup()
{
  // port/pin assignments- set all pins to output- more info here: http://www.arduino.cc/en/Reference/PortManipulation
  DDRB = 0xFF; // port b- digital pins 8-13
  DDRC = 0xFF; // port c- anlog pins 0-5
  DDRD = 0xFF; // port d- digital pins 0-7
  //the three lines above are the same as setting all pins as outputs using arduino library:
  //  pinMode(A0, OUTPUT);
  //  pinMode(A1, OUTPUT);
  //  pinMode(A2, OUTPUT);
  //  pinMode(A3, OUTPUT);
  //  pinMode(A4, OUTPUT);
  //  pinMode(A5, OUTPUT);
  //  for (int pin=0; pin < 14; pin++)
  //    pinMode(pin, OUTPUT);
  // run intialization so we know device is working- leds should light up in order from top of wand to bottom
  // for each time step
  for (byte j = 0; j < 20; j++)
  {
    // for first six rows of data
    for (byte i = 0; i < 6; i++)
    {
      data1 = data1 << 1; // bitwise shift left
      data1 |= load[(i * 20 + j)]; // add next value from dataset
    }
    // for next eight rows of data
    for (byte i = 6; i < 14; i++)
    {
      data2 = data2 << 1; // bitwise shift left
      data2 |= load[(i * 20 + j)]; // add next value from dataset
    }
    // for next six rows of data
    for (byte i = 14; i < 20; i++)
    {
      data3 = data3 << 1; // bitwise shift left
      data3 |= load[(i * 20 + j)]; // add next value from dataset
    }
    PORTB = data1;
    PORTD = data2;
    PORTC = data3;
    delay(100);
  }
#if 0
  //  // SAME AS ABOVE LOOP, BUT USING ARDUINO LIBRARY:
  //  // turn on each LED one by one using arduino library commands
  //  // turn on each digital pin sequentially for 100ms
  //  for (int pin= 13; pin>=0; pin--)
  //  {
  //    digitalWrite(pin, HIGH);
  //    delay(100);
  //    digitalWrite(pin,LOW);
  //  }
  //  // the following lines turn on each analog pin individually
  //  digitalWrite(A5, HIGH);
  //  delay(100);
  //  digitalWrite(A5,LOW);
  //  digitalWrite(A4, HIGH);
  //  delay(100);
  //  digitalWrite(A4,LOW);
  //  digitalWrite(A3, HIGH);
  //  delay(100);
  //  digitalWrite(A3,LOW);
  //  digitalWrite(A2, HIGH);
  //  delay(100);
  //  digitalWrite(A2,LOW);
  //  digitalWrite(A1, HIGH);
  //  delay(100);
  //  digitalWrite(A1,LOW);
  //  digitalWrite(A0, HIGH);
  //  delay(100);
  //  digitalWrite(A0,LOW);
#endif
  // clear data storage
  data1 = 0;
  data2 = 0;
  data3 = 0;
  // clear ports- set all arduino pins to 0Volts
  PORTB = data1;
  PORTD = data2;
  PORTC = data3;
}

void loop()
{
  // space at beginning of text
  PORTB = 0;
  PORTD = 0;
  PORTC = 0;
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
    else if (povtext.charAt(n) == 'M') sendToWandMW(letterM);
    else if (povtext.charAt(n) == 'N') sendToWand(letterN);
    else if (povtext.charAt(n) == 'O') sendToWand(letterO);
    else if (povtext.charAt(n) == 'P') sendToWand(letterP);
    else if (povtext.charAt(n) == 'Q') sendToWand(letterQ);
    else if (povtext.charAt(n) == 'R') sendToWand(letterR);
    else if (povtext.charAt(n) == 'S') sendToWand(letterS);
    else if (povtext.charAt(n) == 'T') sendToWand(letterT);
    else if (povtext.charAt(n) == 'U') sendToWand(letterU);
    else if (povtext.charAt(n) == 'V') sendToWand(letterV);
    else if (povtext.charAt(n) == 'W') sendToWandMW(letterW);
    else if (povtext.charAt(n) == 'X') sendToWand(letterX);
    else if (povtext.charAt(n) == 'Y') sendToWand(letterY);
    else if (povtext.charAt(n) == 'Z') sendToWand(letterZ);
    else if (povtext.charAt(n) == ' ')
    {
      PORTB = 0;
      PORTD = 0;
      PORTC = 0;
      delay(refreshrate * 3); //off for 3 pixels
    }
    //space between each character
    PORTB = 0;
    PORTD = 0;
    PORTC = 0;
    delay(refreshrate);
  }
  //space at end of text
  PORTB = 0;
  PORTD = 0;
  PORTC = 0;
  delay(refreshrate * 3);
}

