// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
// 2010-02-04 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

// contributed by @mariusster, see http://forum.jeelabs.net/comment/1902

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

RTC_PCF8563 RTC;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int myHo;
int myMi;
int mySe;
int myDa;
int myMo;
int myYe;

void setup ()
{
//    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
    lcd.begin(16, 2);
//    RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop ()
{
    DateTime now = RTC.now();
    lcd.setCursor(0, 0);
    myHo = now.hour();
    myMi = now.minute();
    mySe = now.second();
    myDa = now.day();
    myMo = now.month();
    myYe = now.year();
    if (myHo < 10)
      lcd.print('0');
    lcd.print(myHo);
    lcd.print(':');
    if (myMi < 10)
      lcd.print('0');
    lcd.print(myMi);
    lcd.print(':');
    if (mySe < 10)
      lcd.print('0');
    lcd.print(mySe);
    lcd.setCursor(0, 1);
    if (myDa < 10)
      lcd.print('0');
    lcd.print(myDa);
    lcd.print('/');
    if (myMo < 10)
      lcd.print('0');
    lcd.print(myMo);
    lcd.print('/');
    lcd.print(myYe);
/*    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    */
    //delay(1000);
}

