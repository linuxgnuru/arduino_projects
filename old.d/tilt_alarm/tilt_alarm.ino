void setup() {
  // put your setup code here, to run once:
pinMode(8, INPUT);
pinMode(9, OUTPUT);
pinMode(10, OUTPUT);
pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (digitalRead(8) == LOW)
  {
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  else
  {
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
  }
  delay(100); 
}

