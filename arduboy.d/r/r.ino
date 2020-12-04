unsigned long lm = 0;

void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long cm = millis();
  if (cm - lm >= 1000)
  {
    lm = cm;
    ra();
  }
}

void ra()
{
  char dir, rid;
  char d, r;
  rid = random(4);
  dir = (1 << rid);
  r = rand() % 4;
  d = (1 << r);
  Serial.print(F("dir: "));
  Serial.print(dir, DEC);
  Serial.print(F(" rid: "));
  Serial.println(rid, DEC);
  Serial.print(F("  d: "));
  Serial.print(d, DEC);
  Serial.print(F("   r: "));
  Serial.println(r, DEC);
}

