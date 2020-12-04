void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  //  Serial.begin(9600);
}

void loop()
{
  blink(10, 1000);
}

void blink(int cnt, int d)
{
  for (int i = 0; i < cnt; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(d);
    digitalWrite(LED_BUILTIN, LOW);
    delay(d);
  }
}

