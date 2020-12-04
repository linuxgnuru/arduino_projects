const unsigned long Delay = 1000;
unsigned long lastMillis = 0;
byte fu = 1;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  //unsigned long currMillis = millis();
  //fu = 1;
  //if (currMillis - lastMillis >= Delay)  {    lastMillis = currMillis;
    for (int i = 0; i < 8; i++)
    {
      Serial.print(F("fu before: ")); Serial.print(fu, BIN);
      fu = fu >> 1;
      Serial.print(F(" fu after: ")); Serial.println(fu, BIN);
    }
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
 // }
}
