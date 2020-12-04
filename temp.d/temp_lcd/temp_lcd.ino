/*
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
 
 */

#include <LiquidCrystal.h>
#include <math.h>
#include <Wire.h>
//#include <RTClib.h>

//RTC_Millis RTC;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int cf = 0;

void setup()
{
//  RTC.begin(DateTime(__DATE__, __TIME__));
  lcd.begin(16, 2);
  pinMode(7, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
//  DateTime now = RTC.now();
  lcd.setCursor(0, 0);
  lcd.print("Temperature");
  if (digitalRead(7) == HIGH)
   cf = !cf;
  lcd.setCursor(0, 1);
  lcd.print(Thermistor(analogRead(A0)));
  lcd.print((cf == 1 ? " F" : " C"));
  /*
  if (now.day() < 10)
    lcd.print('0');
  lcd.print(now.day());
  lcd.print('/');
  if (now.month() < 10)
    lcd.print('0');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
  */
  delay(300);
}

double Thermistor(int RawADC)
{
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1))); 
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  if (cf == 1)
   Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 return Temp;
}
