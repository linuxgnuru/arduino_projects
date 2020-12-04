int dataPin = 3;
int latchPin = 4;
int clockPin = 5;

void setup()
{
  pinMode(latchPin, OUTPUT);
}

byte data[] = {
  0b00000000, 0b00000000, 0b00000000
};

void loop()
{
  for (int i = 0; i < 20; i++)
  {
    digWrite(i, HIGH);
    delay(25);
    digWrite(i, LOW);
  }
}

void digWrite(int pin, int val)
{
  if (pin > -1 && pin < 8)
  {
    if (val == HIGH) bitSet(data[0], pin);
    else bitClear(data[0], pin);
  }
  else if (pin > 7 && pin < 16)
  {
    if (val == HIGH) bitSet(data[1], abs(pin - 8));
    else bitClear(data[1], abs(pin - 8));
  }
  else if (pin > 15 && pin < 20)
  {
    if (val == HIGH) bitSet(data[2], abs(pin - 16));
    else bitClear(data[2], abs(pin - 16));
  }
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 3; i++)
    shiftOut(dataPin, clockPin, data[i]);
  digitalWrite(latchPin, 1);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int i = 0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    pinState = (myDataOut & (1 << i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

