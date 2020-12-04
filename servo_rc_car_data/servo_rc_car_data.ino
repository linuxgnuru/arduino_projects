
unsigned long lastMillis = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis = millis();
  float R1 = 10;
  float R2 = 0;
  float Vin = 5.0;

  // only run every 1 second
  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    int raw = analogRead(A1);            // number from 0 to 1023
    float Vout = (5.0 / 1023.0) * raw;   // voltage after the resistor
    float buffer = (Vin / Vout) - 1;     // difference in voltages before and after the resistors
    R2 = (R1 / buffer);                  // resistance (ohms) of R2
    Serial.println(R2);
  }
}

