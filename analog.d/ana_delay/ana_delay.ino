long ana;
int i;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (i = 0; i < 6; i++)
  {
    ana = analogRead(i);
    Serial.print(ana);
    Serial.print(" ");
  }
  Serial.println();
  delay(400);
}
