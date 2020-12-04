const int aPin = A2;

int maxN = 0;
int minN = 1023;

void setup()
{
  Serial.begin(115200);
  pinMode(aPin, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  //  calibrate();
}

void loop()
{
  // #if 0
  if (digitalRead(7) == LOW)
  {
    maxN = 0;
    minN = 1023;
    Serial.println("-- RESET --");
  }
  calibrate();
  //   #endif
#if 0
  int a;
  a = analogRead(aPin);
  delay(10);
  a = analogRead(aPin);
  delay(10);
  if (a < 900)
  {
    Serial.println(a);
  }
#endif
  delay(500);
}

void calibrate()
{
  int val;
  for (int i = 0; i < 100; i++)
  {
    val = analogRead(aPin);
    delay(10);
    val = analogRead(aPin);
    delay(10);
    if (val < 900)
    {
      if (minN > val)
        minN = val;
      if (maxN < val)
        maxN = val;
    }
  }
  if (val < 900 && (maxN < 1000 && minN < 1000))
  {
    Serial.print("max ");
    Serial.print(maxN);
    Serial.print(" min ");
    Serial.println(minN);
  }
}

int readInput(int pin)
{
  long c;

  c = analogRead(pin);
  return (c > 1000 ? 0 :
          (c > 400 && c < 500 ? 1 :
           (c > 300 && c < 370 ? 2 :
            (c < 20 ? 3 : 0))));
  /*
    return (c > 1000 ? 0 :
            (c > 440 && c < 470 ? 1 :
             (c < 400 && c > 370 ? 2 :
              (c > 280 && c < 310 ? 3 :
               (c > 150 && c < 180 ? 4 :
                (c < 20 ? 5 : 0))))));
  */
}

