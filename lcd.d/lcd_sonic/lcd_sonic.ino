/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
*/
/*
  HC-SR04 Ping distance sensor:
  VCC to arduino 5v
  GND to arduino GND
  Echo to Arduino pin 7
  Trig to Arduino pin 8

  This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
  Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
  And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
  on 10 Nov 2012.
*/

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int echoPinA = 7; // Echo Pin for device A
const int trigPinA = 8; // Trigger Pin for device A
const int echoPinB = 9; // Echo Pin for device B
const int trigPinB = 10; // Trigger Pin for device B
long durationA; // Duration used to calculate distance A
long durationB; // Duration used to calculate distance B
long cmA, inchesA, cmB, inchesB;
/*
  int maximumRange = 200; // Maximum range needed
  int minimumRange = 0; // Minimum range needed
*/

void setup()
{
  pinMode(trigPinA, OUTPUT);
  pinMode(echoPinA, INPUT);

  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);
  lcd.begin(16, 2);
}

void loop()
{
  durationA = getDistance(trigPinA, echoPinA);
  // convert the time into a distance
  cmA = (durationA / 2) / 29.1;
  inchesA = (durationA / 2) / 74;

  durationB = getDistance(trigPinB, echoPinB);
  cmB = (durationB / 2) / 29.1;
  inchesB = (durationB / 2) / 74;
  lcd.setCursor(0, 0);
  lcd.print("A ");
  if (cmA < 200)
  {
    if (inchesA < 10)
      lcd.print("0");
    lcd.print(inchesA);
    lcd.print("\", ");
    if (cmA < 10)
      lcd.print("0");
    lcd.print(cmA);
    lcd.print("cm");
  }
  lcd.setCursor(0, 1);
  lcd.print("B ");
  if (cmA < 200)
  {
    if (inchesB < 10)
      lcd.print("0");
    lcd.print(inchesB);
    lcd.print("\", ");
    if (cmB < 10)
      lcd.print("0");
    lcd.print(cmB);
    lcd.print("cm");
  }
  delay(250);
}

long getDistance(int tp, int ep)
{
  long d;
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(tp, LOW);
  delayMicroseconds(5);
  digitalWrite(tp, HIGH);
  delayMicroseconds(10);
  digitalWrite(tp, LOW);
  /* Read the signal from the sensor: a HIGH pulse whose
     duration is the time (in microseconds) from the sending
     of the ping to the reception of its echo off of an object. */
  pinMode(ep, INPUT);
  d = pulseIn(ep, HIGH);
  return d;
}

#if 0
double Thermistor(int RawADC)
{
  double Temp;
  Temp = log(10000.0 * ((1024.0 / RawADC - 1)));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
  if (cf == FAR)
    Temp = (Temp * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit
  return Temp;
}
#endif

