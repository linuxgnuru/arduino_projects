const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

byte data[] = {
  0b00000000, 0b00000000, 0b00000000
};

byte chaser[][3] = {
  //                 1              2
  //  01234567     89012345     67890
  { 0b10000000,  0b00000000,  0b00000000 }, // 0
  { 0b01000000,  0b00000000,  0b00000000 }, // 1
  { 0b00100000,  0b00000000,  0b00000000 }, // 2
  { 0b00010000,  0b00000000,  0b00000000 }, // 3
  { 0b00001000,  0b00000000,  0b00000000 }, // 4
  { 0b00000100,  0b00000000,  0b00000000 }, // 5
  { 0b00000010,  0b00000000,  0b00000000 }, // 6
  { 0b00000001,  0b00000000,  0b00000000 }, // 7
  { 0b00000000,  0b10000000,  0b00000000 }, // 8
  { 0b00000000,  0b01000000,  0b00000000 }, // 9
  { 0b00000000,  0b00100000,  0b00000000 }, // 10
  { 0b00000000,  0b00010000,  0b00000000 }, // 11
  { 0b00000000,  0b00001000,  0b00000000 }, // 12
  { 0b00000000,  0b00000100,  0b00000000 }, // 13
  { 0b00000000,  0b00000010,  0b00000000 }, // 14
  { 0b00000000,  0b00000001,  0b00000000 }, // 15
  { 0b00000000,  0b00000000,  0b10000000 }, // 16
  { 0b00000000,  0b00000000,  0b01000000 }, // 17
  { 0b00000000,  0b00000000,  0b00100000 }, // 18
  { 0b00000000,  0b00000000,  0b00010000 }  // 19
};

byte d[] = { 0b00000000, 0b00000000, 0b00000000 };

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

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
//  Serial.begin(9600);
#if 0
  d[0] = d[1] = d[2] = 0b00000000;
//           76543210
  //d[2] = 0b00010000;
  for (int j = 0; j < 3; j++)
  {
  for (int i = 0; i < 8; i++)
  {
    bitSet(d[j], i);
    digitalWrite(latchPin, 0);
    for (int i = 0; i < 3; i++)
      shiftOut(dataPin, clockPin, d[i]);
    digitalWrite(latchPin, 1);
    delay(250);
  }
  delay(250);
  }
#endif
}

void loop()
{
  for (int i = 0; i < 20; i++)
  {
     digWrite(i, HIGH);
     delay(100);
  }
  for (int i = 20; i > -1; i--)
  {
     digWrite(i, LOW);
     delay(100);
  }
}

void digWrite(int pin, int val)
{
  if (pin > -1 && pin < 8)
  {
    if (val == HIGH) bitSet(d[0], pin);
    else bitClear(d[0], pin);
  }
  else if (pin > 7 && pin < 16)
  {
    if (val == HIGH) bitSet(d[1], abs(pin - 8));
    else bitClear(d[1], abs(pin - 8));
  }
  else if (pin > 15 && pin < 20)
  {
    if (val == HIGH) bitSet(d[2], abs(pin - 16));
    else bitClear(d[2], abs(pin - 16));
  }
  digitalWrite(latchPin, 0);
  for (int i = 0; i < 3; i++)
    shiftOut(dataPin, clockPin, d[i]);
  digitalWrite(latchPin, 1);
}

#if 0
/*
byte d[3];

for (int i = 0; i < 21; i++)
{
  for (int j = 0; j < 3; j++)
    d[j] = chaser[i][j];
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, d[0]);
  shiftOut(dataPin, clockPin, d[1]);
  shiftOut(dataPin, clockPin, d[2]);
  digitalWrite(latchPin, 1);
  delay(50);
}
for (int i = 20; i > -1; i--)
{
  for (int j = 2; j > -1; j--)
    d[j] = chaser[i][j];
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, d[0]);
  shiftOut(dataPin, clockPin, d[1]);
  shiftOut(dataPin, clockPin, d[2]);
  digitalWrite(latchPin, 1);
  delay(50);
}
*/

byte d = 0b00000000;
int pos = 0;

for (int i = 0; i < 3; i++)
{
  for (int j = 0; j < 8; j++)
  {
    bitSet(data[i], j);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data[0]);
    shiftOut(dataPin, clockPin, data[1]);
    shiftOut(dataPin, clockPin, data[2]);
    digitalWrite(latchPin, 1);
    delay(100);
    bitClear(data[i], j);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data[0]);
    shiftOut(dataPin, clockPin, data[1]);
    shiftOut(dataPin, clockPin, data[2]);
    digitalWrite(latchPin, 1);
    delay(100);
  }
}

//#if 0
byte d;
int index;
int srpos = 0;

for (int i = 0; i < 20; i++)
{
  index = i % 8;
  if (i < 8)
    srpos = 0;
  if (i > 7 && i < 16)
    srpos = 1;
  if (i > 15)
    srpos = 2;
  bitSet(data[srpos], index);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data[0]);
  shiftOut(dataPin, clockPin, data[1]);
  shiftOut(dataPin, clockPin, data[2]);
  digitalWrite(latchPin, 1);
  delay(100);
  bitClear(data[srpos], index);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data[0]);
  shiftOut(dataPin, clockPin, data[1]);
  shiftOut(dataPin, clockPin, data[2]);
  digitalWrite(latchPin, 1);
  delay(100);
}
}
#endif

