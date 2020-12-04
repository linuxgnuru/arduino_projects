int sensorPin = 5;
int value = 0;

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, INPUT);
  Serial.begin(9600);
}
void loop()
{
  if (digitalRead(8) == HIGH)
  {
    digitalWrite(6, !digitalRead(6));
  }
  value = analogRead(0);
  Serial.print("X:");
  Serial.print(value, DEC);
  value = analogRead(1);
  Serial.print("| Y:");
  Serial.println(value, DEC);
  /*
  value = digitalRead(7);
  Serial.print("| Z:");
  Serial.println(value, DEC);
  */
  delay(100);
}
