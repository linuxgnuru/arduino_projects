int i = 0;
int degree = 25;
int nothing = 100;
void setup()
{
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  // Using Serial.begin(9600) makes pin 1 stay HIGH for some reason
}

void loop()
{
  //NUMERAL PINS ARE 4, 5, and 9
  //   4 LOW = FIRST NUMERAL
  //   5 LOW = SECOND NUMERAL
  //   9 LOW = THIRD NUMERAL

  //FIRST NUMERAL ON THE DISPLAY
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(9, HIGH);
  letterfunction(7); // type 0-9 in the parenthesis or
  //type (degree) for the degree symbol
  // or type (nothing) for nothing
  //NUMERAL 2
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(9, HIGH);
  letterfunction(7);
  //NUMERAL 3
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(9, LOW);
  letterfunction(degree); // the degree symbol
}

//------------------------------------------------------------
// THIS FUNCTION HAS THE DIFFERENT NUMBERS
// AND THE DEGREE SYMBOL
void letterfunction(unsigned char var)
{
  if (var == 1)
  {
    digitalWrite(6, HIGH); //pins 6 and 2 are the leftmost vertical lines
    digitalWrite(2, HIGH);
    delay(5);
    for (i = 1; i < 11; i++)
    {
      digitalWrite(i, LOW);
    }
  }
  if (var == 2)
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(10, HIGH);
    delay(5);
    for (i = 1; i < 11; i++)
    {
      digitalWrite(i, LOW);
    }
  }
  if (var == 3)
  {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(10, HIGH);
    delay(5);
    for (i = 1; i < 11; i++)
    {
      digitalWrite(i, LOW);
    }
  }
  if (var == 4)
  {
    digitalWrite(6, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(1, HIGH);
    delay(5);
    for (i = 1; i < 11; i++)
    {
      digitalWrite(i, LOW);
    }
  }
  if (var == 5)
  {
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(10, HIGH);
    delay(5);
    for (i = 1; i < 11; i++)
    {
      digitalWrite(i, LOW);
    }
  }
  if (var == 6)
  {
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(10, HIGH);
    delay(5);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);
    digitalWrite(10, LOW);

  }

  if (var == 7) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(1, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  if (var == 8) {
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  if (var == 9) {
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(1, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  if (var == 0) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(6, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  if (var == degree) { //the degree symbol
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }
}
