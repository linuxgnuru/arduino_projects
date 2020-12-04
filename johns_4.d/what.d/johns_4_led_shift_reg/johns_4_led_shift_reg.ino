
#define DIG_NUM 4

// set the anode interface
const int dataPin  = 11; // Pin connected to DS of 74HC595 - blue
const int latchPin = 10; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = LED_BUILTIN; // Pin connected to SH_CP of 74HC595 - yellow

int cols[DIG_NUM];

byte data = 0b00000000;

// set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock

const byte ledNum[] = {
  //abcdefg
  0b11111100, // 0
  0b01100000, // 1
  0b11011010, // 2
  0b11110010, // 3
  0b01100110, // 4
  0b10110110, // 5
  0b10111110, // 6
  0b11100000, // 7
  0b11111110, // 8
  0b11110110  // 9
};

int d4 = 3;
int d3 = 4;
int d2 = 5;
int d1 = 6;

void setup()
{
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

void loop()
{
  clearLeds();
  pickDigit(4);
  pickNumber((n / x / 1000) % 10, false);
  //  delayMicroseconds(del);
  clearLeds();
  pickDigit(3);
  pickNumber((n / x / 100) % 10, false);
  //  delayMicroseconds(del);
  clearLeds();
  pickDigit(2);
  pickNumber((n / x / 10) % 10, true);
  //  delayMicroseconds(del);
  clearLeds();
  pickDigit(1);
  pickNumber(n / x % 10, false);
  //  delayMicroseconds(del);
  n++;
  //  if (digitalRead(13) == LOW) n = 0;
}

void pickDigit(int x)  //define pickDigit(x),to open dx port
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  switch (x)
  {
    case 1:
      digitalWrite(d1, LOW);
      break;
    case 2:
      digitalWrite(d2, LOW);
      break;
    case 3:
      digitalWrite(d3, LOW);
      break;
    default:
      digitalWrite(d4, LOW);
      break;
  }
}

void clearLeds()
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, 0b00000000);
  digitalWrite(latchPin, 1);
}

void pickNumber(int x, boolean dp)   //define pickNumber(x)to display number x
{
  digitalWrite(latchPin, 1);
  data = ledNum[x];
  if (dp == true) bitSet(data, 0);
  else bitClear(data, 0);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, 1);
}

#if 0
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
//#endif

//#if 0
void digWrite(int pin, int val)
{
  pin = abs(pin - 12);
  if (val == HIGH)
    bitSet(data, pin);
  else
    bitClear(data, pin);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data);
  digitalWrite(latchPin, 1);
}
#endif
