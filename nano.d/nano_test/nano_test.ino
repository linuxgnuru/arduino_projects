void setup() {
  // put your setup code here, to run once:
pinMode(LED_BUILTIN, OUTPUT);
Serial.begin(9600);
}

int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
 Serial.print("i: ");
 Serial.println(i++);
 digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
 delay(500);
}
