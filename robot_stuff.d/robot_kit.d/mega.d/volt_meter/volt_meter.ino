const int voltPin = A1;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  float vin = 0.0;
  vin = getVolt(voltPin);
  Serial.println(vin);
  delay(1000);
}

float getVolt(int aPin)
{
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 100000.0;
  float R2 = 10000.0;
  int value;

  value = analogRead(aPin);
  vout = (value * 5.0) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.09)
    vin = 0.0;
  return vin;
}

