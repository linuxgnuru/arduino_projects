void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  long i = analogRead(A2);
  if (i < 1000) {
    Serial.println(i);
    delay(500);
  }
}
