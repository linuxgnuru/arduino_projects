int cnt = 0;

void setup()
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  //Serial.begin(9600);
}

void loop()
{
  if (digitalRead(7) == LOW)
  {
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
  }
  else
  {
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
  }
  if (digitalRead(6) == LOW)
  {
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
  }
  else
  {
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
  }
  //Serial.println(digitalRead(7));
  /*
  Serial.print("cnt:");
  Serial.println(cnt++);
  */
  delay(500);
}
