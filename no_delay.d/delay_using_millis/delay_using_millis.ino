// attempt at millis for true delay
const unsigned long M = 1000;
unsigned long lastMillis = 0;
boolean cont_flag = false;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  unsigned long currMillis;

  while (cont_flag == false)
  {
    currMillis = millis();
    if (currMillis - lastMillis >= M)
    {
      lastMillis = currMillis;
      cont_flag = true;
    }
  }
  cont_flag = false;
}

