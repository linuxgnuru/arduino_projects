//set the anode interface
const int dataPin  = 11; // Pin connected to DS of 74HC595 - blue
const int latchPin = 10; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = LED_BUILTIN; // Pin connected to SH_CP of 74HC595 - yellow

byte d = 0b00000000;

//set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock

const byte digitArray[] = {
//  BCDGE.FA
  0b01111011, // 0
  0b00001010, // 1
  0b11110010, // 2
  0b10111010, // 3
  0b10001011, // 4
  0b10111001, // 5
  0b11111001, // 6
  0b00011010, // 7
  0b11111011, // 8
  0b10111011  // 9
};

// for shift register leds:
byte data[] = { 
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

void setup()
{
  pinMode(latchPin, OUTPUT);
}

void loop()
{
  printDigit(0, (n / x / 1000) % 10, false);
  printDigit(1, (n / x / 100) % 10, false);
  printDigit(2, (n / x / 10) % 10, true);
  printDigit(3, n / x % 10, false);
  delayMicroseconds(del);
  n++;
  if (digitalRead(3) == LOW)
  {
    n = 0;
  }
}

void printDigit(int pos, int num, boolean dp)
{
  int ctr = 0;
  byte myData;

  if (pos < 4 && (num > -1 && num < 10 || num == 99))
  {
    myData = (num == 99 ? 0b00000000 : digitArray[num]);
    if (dp == true)
      bitSet(myData, 2);
    data[pos] = myData;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    digitalWrite(latchPin, 1);
  }
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}


