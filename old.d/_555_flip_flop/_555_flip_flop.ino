void setup()
{
  pinMode(12, INPUT);
  pinMode(9, INPUT);
  pinMode(8, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    if (digitalRead(12) == LOW)
    {
      digitalWrite(8,
    }
    if (digitalRead(
    delay(30);
}
