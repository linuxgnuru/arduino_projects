long ana;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  ana = analogRead(A1);
  Serial.println(ana);
  delay(500);
}
