const byte relay = 13;
const byte shock = 7;
int val;

unsigned long lastMillis = 0;
boolean delayFlag = true;

void setup() {
  pinMode(relay, OUTPUT);
  pinMode(shock, INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  val = digitalRead(shock);
  if (currentMillis - lastMillis <= 3000 && delayFlag == false) {
    lastMillis = currentMillis;
    delayFlag = true;
  }
  if (val == HIGH && delayFlag == true) {
    digitalWrite(relay, LOW);
  } else {
    digitalWrite(relay, HIGH);
    delayFlag = false;
  }
}

