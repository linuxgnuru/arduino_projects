void setup() {
  // put your setup code here, to run once:
pinMode(12, INPUT);
pinMode(11, INPUT);
pinMode(10, OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (digitalRead(12) == HIGH)
  {
    digitalWrite(10, HIGH);
  }
  if (digitalRead(11) == HIGH)
  {
    digitalWrite(10, LOW);
  }
}
