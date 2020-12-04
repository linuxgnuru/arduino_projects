const int rgb[] = {
  13, 12, 11
};

boolean rgb_b[] = {
  0, 1, 1
};

void setup()
{
  for (int i = 0; i < 3; i++)
    pinMode(rgb[i], OUTPUT);
}

void loop()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(rgb[i], !digitalRead(rgb[i]));
    delay(1000);
    digitalWrite(rgb[i], !digitalRead(rgb[i]));
    delay(1000);
  }
}

