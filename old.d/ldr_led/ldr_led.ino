void setup()
{
   pinMode(9, OUTPUT);
   pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  analogWrite(9, map(analogRead(A6), 0, 1023, 0, 255));
 // delay(250);
}
