// for MAN71A 7 segment LED

/*
     A
 F       B
     G   
 E       C
     D      DP
*/

int pinA = 10;
int pinB = 7;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 9;
int pinG = 8;
int pinDP = 3;

int i;
int j;

int myDelay = 400;

void setup()
{
  for (i = 3; i < 14; i++)
    pinMode(i, OUTPUT);
  // put your setup code here, to run once:

}

void loop()
{
  clearLed();
  for (j = 0; j < 10; j++)
  {
    printNum(j);
    delay(myDelay);
  }
  clearLed();
  delay(300);
  for (j = 9; j > -1; j--)
   {
     printNum(j);
     delay(myDelay);
   }
}

void printNum(int num)
{
  switch (num)
  {
    case 0:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, LOW);
      break;
    case 1:
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;
    case 2:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinC, LOW);
      digitalWrite(pinF, LOW);
      break;
    case 3:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      break;
    case 4:
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinA, LOW);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      break;
    case 5:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinB, LOW);
      digitalWrite(pinE, LOW);
      break;
    case 6:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinB, LOW);
      digitalWrite(pinE, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinF, HIGH);
      break;
    case 7:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, LOW);
      digitalWrite(pinE, LOW);
      digitalWrite(pinF, LOW);
      digitalWrite(pinG, LOW);
      break;
    case 8:
      for (i = 3; i < 11; i++)
        digitalWrite(i, HIGH);
      break;
    case 9:
      digitalWrite(pinA, HIGH);
      digitalWrite(pinF, HIGH);
      digitalWrite(pinG, HIGH);
      digitalWrite(pinB, HIGH);
      digitalWrite(pinC, HIGH);
      digitalWrite(pinD, HIGH);
      digitalWrite(pinE, LOW);
      break;
  }
}

void clearLed()
{
  for (i = 3; i < 11; i++)
    digitalWrite(i, LOW);
}
