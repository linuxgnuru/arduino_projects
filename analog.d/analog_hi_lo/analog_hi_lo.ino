void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  digitalWrite(LED_BUILTIN, (analogRead(A0) > 511 ? HIGH : LOW));
  delay(100);
}
