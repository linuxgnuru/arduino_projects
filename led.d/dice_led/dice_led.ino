/*
#include <Wire.h>
#include <pcf8574.h>

PCF8574 PCF_20(0x20);
*/

int i;
long ran;
int oneDieButState;
int twoDiceButState;
int myDelay = 10;
int j;

void setup()
{
//  Serial.begin(57600);
//  Wire.begin();
  for (i = 5; i < 14; i++)
    pinMode(i, OUTPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  randomSeed(analogRead(A0));
  digitalWrite(LED_BUILTIN, LOW);
/*
  for (i = 0; i < 7; i++)
  {
    ran = i;
    printDie();
    delay(400);
    for (j = 5; j < 12; j++)
      digitalWrite(j, LOW);
  }
  */  
/*
  for (i = 0; i < 7; i++)
  {
    PCF_20.write(i, 0);
    delay(100);
  }
  delay(500);
  for (i = 6; i < 13; i++)
  {
    digitalWrite(i, LOW);
    delay(100);
  }
  for (i = 6; i > -1; i--)
  {
    PCF_20.write(i, 1);
    delay(100);
  }
  */
}

void loop()
{
  oneDieButState = digitalRead(2);
  twoDiceButState = digitalRead(3);
  if (oneDieButState == HIGH)
  {
    digitalWrite(12, LOW);
    myDelay = 50;
    for (i = 0; i < 30; i++)
    {
      ran = random(1, 7);
      for (j = 5; j < 12; j++)
        digitalWrite(j, LOW);
      printDie();
      delay(myDelay);
      myDelay += 5;
    }
    ran = random(1, 7);
    for (j = 5; j < 12; j++)
      digitalWrite(j, LOW);
    printDie();
    digitalWrite(12, HIGH);
//    delay(4000);
    /*
    for (i = 5; i < 12; i++)
      digitalWrite(i, LOW);
      */
  }
/*
  else if (twoDiceButState == HIGH)
  {
    myDelay = 10;
    for (i = 0; i < 10; i++)
    {
      ran = random(1, 7)
      printDie();
      delay(myDelay);
      myDelay *= 4;
    }
  }
*/
}

void printDie()
{
  switch (ran)
  {
      case 1:
        digitalWrite(8, HIGH);
        break;
      case 2:
        digitalWrite(11, HIGH);
        digitalWrite(5, HIGH);
        break;
      case 3:
        digitalWrite(11, HIGH);
        digitalWrite(8, HIGH);
        digitalWrite(5, HIGH);
        break;
      case 4:
        digitalWrite(9, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        break;
      case 5:
        digitalWrite(11, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(8, HIGH);
        break;
      case 6:
        digitalWrite(11, HIGH);
        digitalWrite(9, HIGH);
        digitalWrite(5, HIGH);
        digitalWrite(6, HIGH);
        digitalWrite(7, HIGH);
        digitalWrite(10, HIGH);
        break;
  }
//  PCF_20.write(i, 1);
}
