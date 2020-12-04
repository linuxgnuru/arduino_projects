const int aPins[] = {
  A0, A1, A2, A3, A4, A5
};

//long ana[6];
long ana;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 6; i++)
  {
    ana = analogRead(aPins[i]);
    Serial.print(ana);
    Serial.print("\t");
  }
  Serial.println("");
  delay(100);
}
