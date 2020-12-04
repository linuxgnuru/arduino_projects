#define LED_NUM 10
int pins[LED_NUM];

void setup()
{
  for (int i = 0; i < LED_NUM; i++)
    pins[i] = i + 2;
  for (int i = 0; i < LED_NUM; i++)
    pinMode(pins[i], OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  long val = map(analogRead(A0), 0, 1023, 0, LED_NUM);
  val = constrain(val, 0, LED_NUM);
  val = abs(val - LED_NUM);
  for (int i = 0; i < LED_NUM; i++)
    digitalWrite(pins[i], LOW);
  for (int i = 0; i < val; i++)
    digitalWrite(pins[i], HIGH);
}
