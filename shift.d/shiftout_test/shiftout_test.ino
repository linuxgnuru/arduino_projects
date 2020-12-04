const int dataPin  = 11; // Pin connected to DS of 74HC595 - blue
const int latchPin = 10; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = LED_BUILTIN; // Pin connected to SH_CP of 74HC595 - yellow

void setup()
{
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000001);
  digitalWrite(latchPin, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

}
