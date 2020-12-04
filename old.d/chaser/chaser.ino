void setup()
{
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);
//pinMode(pwmPin, OUTPUT);
pinMode(2, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
/*
  analogWrite(pwmPin, mapData);
  */
  digitalWrite(2, HIGH);
  delay(map(analogRead(A0), 0, 1023, 10, 500));
  digitalWrite(2, LOW);
}
