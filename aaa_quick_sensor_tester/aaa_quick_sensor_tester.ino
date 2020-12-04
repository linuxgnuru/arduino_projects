void setup()
{
  pinMode(8, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //Serial.begin(9600);
}

void loop()
{
//  Serial.println(digitalRead(8));
//  delay(250);
  digitalWrite(LED_BUILTIN, (digitalRead(8) == HIGH ? LOW : HIGH));
}
