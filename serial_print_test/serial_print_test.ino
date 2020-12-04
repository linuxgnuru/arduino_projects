boolean t = true;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int fu = 1000;
  if (t)
  {
    Serial.print(fu / 1000, DEC);
    Serial.print(F(" "));
    Serial.print(fu % 1000, DEC);
    Serial.print(F(" "));
    Serial.print(fu / 100, DEC);
    Serial.print(F(" "));
    Serial.println(fu % 100, DEC);
    t = false;
  }
}

