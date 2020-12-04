void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  digitalWrite(8, HIGH);
  delay(map(analogRead(A0), 0, 1023, 10, 500));
  digitalWrite(8, LOW);
}
