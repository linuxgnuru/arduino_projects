const int ledPins[] = {
  2, 3, 4, 7, 6, 5, 11, 12, LED_BUILTIN 
};

const int colPins[] = {
  8, 9, 10
};

void setup()
{
  for (int i = 2; i < 14; i++)
    pinMode(i, OUTPUT);
}

void loop()
{
  //set pin colPins[0]-colPins[2] high so as to light all the layers
  for (int j = 0; j < 4; j++)
  {
    switch (j)
    {
      case 0:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], LOW);
        break;
      case 1:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], LOW);
        break;
      case 2:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], HIGH);
        break;
      case 3:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], HIGH);
        break;
      default:
        break;
    }
    digitalWrite(ledPins[4], HIGH);//set the middle column ON throughout
    //light the other columns on and off in a sequence
    for (int i = 0; i < 9; i++)
    {
      if (ledPins[i] != 6)
      {
        digitalWrite(ledPins[i], HIGH);
        delay(100);
        digitalWrite(ledPins[i], LOW);
        delay(100);
      }
    }
  }
  for (int j = 0; j < 4; j++)
  {
    switch (j)
    {
      case 0:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], LOW);
        for (int i = 0; i < 9; i++)
          digitalWrite(ledPins[i], HIGH);
        delay(100);
        break;
      case 1:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], LOW);
        for (int i = 0; i < 9; i++)
          digitalWrite(ledPins[i], HIGH);
        delay(100);
        break;
      case 2:
        digitalWrite(colPins[0], LOW);
        digitalWrite(colPins[1], LOW);
        digitalWrite(colPins[2], HIGH);
        for (int i = 0; i < 9; i++)
          digitalWrite(ledPins[i], HIGH);
        delay(100);
        break;
      case 3:
        digitalWrite(colPins[0], HIGH);
        digitalWrite(colPins[1], HIGH);
        digitalWrite(colPins[2], HIGH);
        for (int i = 0; i < 9; i++)
          digitalWrite(ledPins[i], HIGH);
        delay(100);
        break;
      default:
        break;
    }
    delay(250);
    for (int i = 0; i < 9; i++)
      digitalWrite(ledPins[i], LOW);
  }
  for (int i = 0; i < 3; i++)
    digitalWrite(colPins[i], HIGH);
  for (int j = 0; j < 4; j++)
  {
    face(j);
    delay(250);
    //for (int k = 0; k < 9; k++)      digitalWrite(ledPins[k], LOW);
    //    delayMicroseconds(55);
  }
  for (int i = 0; i < 3; i++)
    digitalWrite(colPins[i], LOW);
}

void face(int Dir)
{
  int a, b, c;
  switch (Dir)
  {
    case 0: // front
      digitalWrite(ledPins[0], LOW);
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[3], LOW);
      digitalWrite(ledPins[4], LOW);
      digitalWrite(ledPins[5], HIGH);
      digitalWrite(ledPins[6], LOW);
      digitalWrite(ledPins[7], LOW);
      digitalWrite(ledPins[8], HIGH);
      break;
    case 1: // right side
      digitalWrite(ledPins[0], LOW);
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[2], LOW);
      digitalWrite(ledPins[3], LOW);
      digitalWrite(ledPins[4], LOW);
      digitalWrite(ledPins[5], LOW);
      digitalWrite(ledPins[6], HIGH);
      digitalWrite(ledPins[7], HIGH);
      digitalWrite(ledPins[8], HIGH);
      break;
    case 2: // back side
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[2], LOW);
      digitalWrite(ledPins[3], HIGH);
      digitalWrite(ledPins[4], LOW);
      digitalWrite(ledPins[5], LOW);
      digitalWrite(ledPins[6], HIGH);
      digitalWrite(ledPins[7], LOW);
      digitalWrite(ledPins[8], LOW);
      break;
    case 3: // left side
      digitalWrite(ledPins[0], HIGH);
      digitalWrite(ledPins[1], HIGH);
      digitalWrite(ledPins[2], HIGH);
      digitalWrite(ledPins[3], LOW);
      digitalWrite(ledPins[4], LOW);
      digitalWrite(ledPins[5], LOW);
      digitalWrite(ledPins[6], LOW);
      digitalWrite(ledPins[7], LOW);
      digitalWrite(ledPins[8], LOW);
      break;
  }
}

