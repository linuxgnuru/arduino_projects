#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const byte trigPin = 5;
const byte echoPin = 4;

unsigned long lastMillis = 0;

void setup()
{
  int error;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.begin(16, 2); // initialize the lcd
}

void loop()
{
  unsigned long currentMillis = millis();
  double duration;
  double cm;
  double inches;

  if (currentMillis - lastMillis >= 500)
  {
    lastMillis = currentMillis;
    //lcd.home();
    //lcd.clear();
    // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Read the signal from the sensor: a HIGH pulse whose
    // duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    // convert the time into a distance
    cm = (duration / 2) / 29.1;
    if (cm < 400)
    {
      for (int i = 0; i < 2; i++)
      {
        lcd.setCursor(15, i);
        lcd.print(" ");
      }
      lcd.setCursor(0, 0);
      if (cm < 100)
      {
        lcd.print(" ");
        if (cm < 10)
          lcd.print(" ");
      }
      lcd.print(cm, 3);
      lcd.print(" cm");
      inches = (duration / 2) / 74;
      lcd.setCursor(0, 1);
      if (inches < 100)
      {
        lcd.print(" ");
        if (inches < 10)
          lcd.print(" ");
      }
      lcd.print(inches, 3);
      lcd.print(" inches");
    }
    else
    {
      for (int i = 0; i < 2; i++)
      {
        lcd.setCursor(15, i);
        lcd.print("*");
      }
    }
    /*
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      //         01234567890123456
      lcd.print("Dist > 400 cm");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    */
  }
}

