const int laserPin = 2;
const int ldrPin = A0;

void setup() {
  // put your setup code here, to run once:
  pinMode(laserPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  long ldr;
  digitalWrite(laserPin, !digitalRead(laserPin));
  ldr = analogRead(ldrPin);
  Serial.println(ldr);
  digitalWrite(LED_BUILTIN, (ldr < 140 ? HIGH : LOW));
  delay(500);
}
