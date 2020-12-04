int i;
int j;
int ana = 0;

void setup()
{
  pinMode(10, OUTPUT);
}

void loop()
{
  ana = analogRead(A0);
//  if
}

void whistle()
{
  for (i = 0; i <= 20; i++)
  {
    for (j = 0; j <= 254; j++)
    {
      digitalWrite(10, 0);
      delayMicroseconds((i*5)+(j * 2));
      digitalWrite(10, 1);
      delayMicroseconds((i*5)+(j * 2));
    }
    PORTB ^= 32; // toggle LED_BUILTIN
  }
}
