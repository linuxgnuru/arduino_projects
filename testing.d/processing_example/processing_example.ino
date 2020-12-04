void setup() { Serial.begin(9600); }
void loop() { Serial.println(abs(analogRead(A0) - 1023)); delay(2); }
