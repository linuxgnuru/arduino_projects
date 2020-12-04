/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int ledState = HIGH;         // the current state of the output pin
int buttonStateA;             // the current reading from the input pin
int lastButtonStateA = LOW;   // the previous reading from the input pin
long lastDebounceTimeA = 0;  // the last time the output pin was toggled

int buttonStateB;             // the current reading from the input pin
int lastButtonStateB = LOW;   // the previous reading from the input pin
long lastDebounceTimeB = 0;  // the last time the output pin was toggled

long debounceDelay = 50;    // the debounce time; increase if the output flickers

int answer = 0;
int question_num = 1;

void setup() {
  lcd.begin(16, 2);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, ledState);

}

void loop()
{
            lcd.print("Are you happy?");
          lcd.setCursor(0, 1);
          lcd.print("1 = yes 2 = no");

  getQ();
  if (answer == 5 || answer == 10)
  {
    switch (question_num)
    {
       case 1:
          if (answer == 5)
          {
            lcd.setCursor(0, 0);
            lcd.print("Very good!");
            lcd.setCursor(0, 1);
            lcd.print("Happy Days!");
          }
          else
          {
            lcd.setCursor(0, 0);
            lcd.print("I'm sorry.");
            lcd.setCursor(0, 1);
            lcd.print("Get a hug.");        
          }
          break;
        default:
            lcd.setCursor(0, 0);
            lcd.print("Sorry, no more");
            lcd.setCursor(0, 1);
            lcd.print("questions");
            break;
    }
  }
  if (answer == 5 || answer == 10)
  {
    delay(2000);
    question_num++;
  }
}

void getQ()
{
  int readingA = digitalRead(5);
  int readingB = digitalRead(6);
  if (readingA != lastButtonStateA)
       lastDebounceTimeA = millis();
  if ((millis() - lastDebounceTimeA) > debounceDelay)
  {
    if (readingA != buttonStateA)
    {
      buttonStateA = readingA;
      if (buttonStateA == HIGH)
        answer = 5;
    }
  } 
  lastButtonStateA = readingA;
  if (readingB != lastButtonStateB)
       lastDebounceTimeB = millis();
  if ((millis() - lastDebounceTimeB) > debounceDelay)
  {
    if (readingB != buttonStateB)
    {
      buttonStateB = readingB;
      if (buttonStateB == HIGH)
        answer = 10;
    }
  } 
  lastButtonStateB = readingB;
}
