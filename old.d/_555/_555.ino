void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);
pinMode(8, INPUT);
//Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(8));
  // put your main code here, to run repeatedly:
//Serial.println(analogRead(A0), DEC);
//delay(400);
}
