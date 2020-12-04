unsigned long lm = 0;

void setup()
{
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
}

boolean runMe = true;

void loop()
{
  unsigned long cm = millis();

  while (runMe)
  {
    digitalWrite(8, LOW);
    if (cm - lm >= 2000)
    {
      lm = cm;
      digitalWrite(8, HIGH);
      runMe = false;
    }
  }
  digitalWrite(8, HIGH);
}

