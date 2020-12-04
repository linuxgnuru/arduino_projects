void setup()
{
  for (int i = 2; i < 6; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

void loop()
{
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, LOW);
    delay(250);
  }
  for (int i = 5; i > 1; i--)
  {
    digitalWrite(i, HIGH);
    delay(250);
  }
  for (int i = 5; i > 1; i--)
  {
    digitalWrite(i, LOW);
    delay(250);
  }
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, HIGH);
    delay(250);
  }
}
