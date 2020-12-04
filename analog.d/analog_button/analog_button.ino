void setup()
{
  Serial.begin(115200);
  pinMode(A2, INPUT_PULLUP);
}

void loop()
{
  int val = 0;
  val = readInput(A2);
  if (val)
    Serial.println(val);
  delay(500);
}

int readInput(int pin)
{
  long c;

  c = analogRead(pin);
  return (c > 1000 ? 0 :
      (c > 440 && c < 470 ? 1 :
      (c < 400 && c > 370 ? 2 :
      (c > 280 && c < 310 ? 3 :
      (c > 150 && c < 180 ? 4 :
      (c < 20 ? 5 : 0))))));
}

