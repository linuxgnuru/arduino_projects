void setup()
{
  pinMode(2, OUTPUT);
  pinMode(4, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    digitalWrite(2, (digitalRead(4) == HIGH ? HIGH : LOW));
}

