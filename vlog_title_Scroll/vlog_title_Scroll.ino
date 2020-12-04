/*
  LiquidCrystal Library - scrollDisplayLeft() and scrollDisplayRight()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 scrollDisplayLeft() and scrollDisplayRight() methods to scroll
 the text.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
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

 http://www.arduino.cc/en/Tutorial/LiquidCrystalScroll

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//          0123456789012345
char a[] = "Electronics,";
char b[] = "computers, & fun";

byte smileyA[8] = {
  0b00000,
  0b00000,
  0b10001,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};
#if 0
byte smileyB[8] = {
  0b00000,
  0b00110,
  0b00110,
  0b00000,
  0b00000,
  0b00011,
  0b11110,
  0b11000
};
#endif
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("Electronics,");
  //lcd.setCursor(0, 1);
  //         0123456789012345
  //lcd.print("computers, & fun");
  //delay(1000);
  lcd.createChar(1, smileyA);
  //lcd.createChar(2, smileyB);
}

const int dl = 250;

void loop()
{
  lcd.setCursor(0, 0);
  for (int i = 0; i < strlen(a); i++)
  {
    lcd.print(a[i]);
    delay(dl);
  }
  lcd.setCursor(0, 1);
  for (int i = 0; i < strlen(b); i++)
  {
    lcd.print(b[i]);
    delay(dl);
  }
  delay(dl+dl);
  lcd.setCursor(13, 0);
  lcd.write((byte) 1);
  //lcd.write(2);
  
  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  //for (int positionCounter = 0; positionCounter < 26; positionCounter++) {
    // scroll one position left:
   // lcd.scrollDisplayLeft();
    // wait a bit:
 //   delay(200);
//  }
/*
  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }
*/
  // delay at the end of the full loop:
  //delay(1000);

}

