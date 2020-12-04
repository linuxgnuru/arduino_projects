void setup() {
  // put your setup code here, to run once:
 pinMode(12, OUTPUT);
 pinMode(11, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly: 
  int out;
  digitalWrite(12, HIGH);
  out = digitalRead(11);
    Serial.print("heart:  ");
    Serial.print(out);
    Serial.println();
    delay(200);
}
