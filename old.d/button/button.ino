void setup() {
  // put your setup code here, to run once:
pinMode(7, OUTPUT);
pinMode(8, INPUT);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (digitalRead(8) == LOW)
  {
    digitalWrite(7, HIGH);
  }
  else
    digitalWrite(7, LOW);
    delay(300);
  // put your main code here, to run repeatedly:

}
