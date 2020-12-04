long ran;
int time = 2000;

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop()
{
  ran = random(1, 7);
  Serial.println(ran);
  delay(time);
}
