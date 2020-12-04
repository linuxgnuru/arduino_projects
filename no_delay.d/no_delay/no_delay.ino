
int ledState = LOW;
long previousMillis = 0;

long interval = 3000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    ledState = (ledState == LOW ? HIGH : LOW);
    digitalWrite(LED_BUILTIN, ledState);
  }
}

