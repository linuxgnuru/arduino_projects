unsigned long lastMillis = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis = millis();
  float R1 = 10;   // First resistor
  float R2 = 0;    // Second resistor
  float Vin = 5.0; // Input voltage

  // only run every second
  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;

    // Raw number from analog input; number ranges from 0 to 1023
    int raw = analogRead(A1);
    
    // Convert the raw value into a voltage.
    float Vout = (5.0 / 1023.0) * raw;
    
    // The equation
    R2 = R1 * (1 / ((Vin / Vout) - 1));
    Serial.println(R2);
  }
}

