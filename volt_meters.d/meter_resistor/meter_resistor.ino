/*
  Wiring:
  5v---*---10k----[Test Resistor]----GND
     = analog connection

*/
// potentiometer wiper (middle terminal) connected to analog pin 0
const byte analogPin = A0;

// outside leads to ground and +5V

unsigned long lastMillis = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  float R1 = 10;         // variable to store the R1 value
  float R2 = 0;          // variable to store the R2 value
  int raw = 0;           // variable to store the raw input value
  int Vin = 5;           // variable to store the input voltage
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillis >= 1000)
  {
    lastMillis = currentMillis;
    // Reads the Input PIN
    raw = analogRead(analogPin);
    // Calculates the Voltage on th Input PIN
    float Vout = (5.0 / 1023.0) * raw;
    // buffer variable for calculation
    float buffer = (Vin / Vout) - 1;
    R2 = R1 / buffer;
    Serial.print(F("Voltage: "));
    Serial.println(Vout);
    Serial.print(F("R2: "));
    Serial.println(R2);
  }
}

