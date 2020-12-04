
long last_result = 2000;
long result;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  result = analogRead(A1);
  if (result != last_result)
  {
    Serial.println(result);
    last_result = result;
  }
  delay(500);
}
