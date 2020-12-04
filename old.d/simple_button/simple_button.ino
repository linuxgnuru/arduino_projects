void setup()
{
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  pinMode(8, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (digitalRead(2) == LOW || digitalRead(3) == LOW)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(8, HIGH);
//    delay(3000);
  }
  else
  {
    digitalWrite(8, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
