/*
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
*/
#include <LiquidCrystal.h>

LiquidCrystal lcd(11, 12, 2, 3, 4, 5);

//          0123456789012345
char a[] = "Electronics,";
char b[] = "computers, & fun";

unsigned long lastMillis = 0;

const int dl = 250;

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

void setup()
{
  lcd.begin(16, 2);
  //         0123456789012345
  lcd.createChar(1, smileyA);
  lcd.createChar(2, smileyB);
}

void loop()
{
  unsigned long curMillis = millis();

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
}

