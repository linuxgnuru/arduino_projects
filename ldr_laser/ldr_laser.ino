unsigned long lM = 0;

void setup()
{
  pinMode(4, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  unsigned long cM = millis();
  if (cM - lM >= 1000)
  {
    lM = cM;
    Serial.println(analogRead(A7));
  }
  digitalWrite(4, (analogRead(A7) > 400));
}
