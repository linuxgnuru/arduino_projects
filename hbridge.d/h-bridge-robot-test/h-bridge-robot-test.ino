void setup()
{
  for (int i = 3; i < 9; i++)
    pinMode(i, OUTPUT);
}

void loop()
{
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);  
}

