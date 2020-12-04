#define DEBUG 1

typedef enum {
  CEL,
  FAR
} scale_enum;

typedef enum {
  TENS,
  ONES,
  DECP
} place_enum;

#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor

const int del = 500; // delay

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow wire

const int col1Pin = 5;
const int col2Pin = 6;
const int col3Pin = 7;

const int cfPin = 12; // cel or far pin
const int alertPin = LED_BUILTIN;

const int analogPin = A0; //the thermistor attach to 

boolean cfBool; // cel or far bool

byte alphabetArray[] = {
  0b11010111, // A
  0b00011111, // B
  0b10001101, // C
  0b01011110, // D
  0b10001111, // E
  0b10000111, // F
  0b10011101, // G
  0b01010111, // H
  0b01010000, // I
  0b01011100, // J
  0b01110111, // K (H.)
  0b00001101, // L
  0b00110110, // M (n.) c b e f
  0b00010110, // N (n)  c b e
  0b11011101, // O
  0b11000111, // P
  0b11111101, // Q (O.)
  0b00000110, // R (r)
  0b10011011, // S
  0b11010000, // T
  0b01011101, // U
  0b00011100, // V
  0b00111100, // W
  0b01010101, // X
  0b01011011, // Y
  0b11001110, // Z (2)
  0b00000000 // space
};

byte digitArray[] = {
  0b11011101, // 0
  0b01010000, // 1
  0b11001110, // 2
  0b11011010, // 3
  0b01010011, // 4
  0b10011011, // 5
  0b10011111, // 6
  0b11010000, // 7
  0b11011111, // 8
  0b11011011  // 9
};

// for shift register leds:
int data;

void setup()
{
  cfBool = FAR;

  Serial.begin(9600);

  pinMode(cfPin, INPUT_PULLUP);

  pinMode(alertPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  pinMode(col1Pin, OUTPUT);
  pinMode(col2Pin, OUTPUT);
  pinMode(col3Pin, OUTPUT);
}

int n = 0;
int x = 100;

void loop()
{
#if 0
  clearLeds();
  pickCol(TENS);
  printDigit((n/x/100)%10, false);
  delayMicroseconds(del);

  clearLeds();
  pickCol(ONES);
  printDigit((n/x/10)%10, false);
  delayMicroseconds(del);

  clearLeds();
  pickCol(DECP);
  printDigit(n/x%10, false);
  delayMicroseconds(del);
  n++;
  if (n >= 1000)
    n = 0;
#endif
#if 0
  float curTemp;
  int tens;
  int ones;
  int decp;
  float tmp;

  //read thermistor value 
  long a = analogRead(analogPin);

  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = ((tempC * 9) / 5) + 32;
  cfBool = (digitalRead(cfPin) == HIGH ? FAR : CEL);
  curTemp = (cfBool == CEL ? tempC : tempF);
  tens = (int)curTemp / 10;
  ones = (int)curTemp % 10;
  tmp = curTemp * 10;
  decp = (int)tmp % 10;
/*
//#ifdef DEBUG
Serial.print("curTemp: ");
Serial.print(curTemp);
Serial.print(" ");
Serial.print(tens);
Serial.print(" ");
Serial.print(ones);
Serial.print(" . ");
Serial.println(decp);
//#endif
*/
/*
    last = (int)tempF % 100;
    dec = tempF - (float)first; // for decimal
    dec *= 100;
    dec *= 10;
    last = (int)dec;
*/
  digitalWrite(col1Pin, LOW);
  digitalWrite(col2Pin, HIGH);
  digitalWrite(col3Pin, HIGH);
  data = digitArray[tens];
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data);
  digitalWrite(latchPin, 1);
//  printDigit(digitArray[tens], false);
  delayMicroseconds(del);

  digitalWrite(col1Pin, HIGH);
  digitalWrite(col2Pin, LOW);
  digitalWrite(col3Pin, HIGH);
  data = digitArray[ones];
  bitSet(data, 5);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data);
  digitalWrite(latchPin, 1);
//  printDigit(digitArray[ones], true);
  delayMicroseconds(del);

  digitalWrite(col1Pin, HIGH);
  digitalWrite(col2Pin, HIGH);
  digitalWrite(col3Pin, LOW);
  data = digitArray[decp];
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data);
  digitalWrite(latchPin, 1);
//  printDigit(digitArray[decp], false);
  delayMicroseconds(del);
  /*
  print3Digit(digitArray[tens], 0, false);
  print3Digit(digitArray[ones], 1, true);
  print3Digit(digitArray[decp], 2, false);
  for (int j = 0; j < 3; j++)
  {
    pickCol(j);
    for (int i = 0; i < 10; i++)
    {
      printDigit(i);
      delay(500);
    }
  }
  */
//  delay(1000);
#endif
delay(1000);
}

/*
void print3digit(int num, int slot, boolean dp_flag)
{
  data = digitArray[num];
  if (dp_flag == true)
    bitSet(data, 3);
  pickCol(slot);
}

void pickCol(int col)
{
  digitalWrite(col1Pin, HIGH);
  digitalWrite(col2Pin, HIGH);
  digitalWrite(col3Pin, HIGH);
  switch (col)
  {
    case TENS:
      digitalWrite(col1Pin, LOW);
      break;
    case ONES:
      digitalWrite(col2Pin, LOW);
      break;
    case DECP:
      digitalWrite(col3Pin, LOW);
      break;
    default:
      digitalWrite(col1Pin, HIGH);
      digitalWrite(col2Pin, HIGH);
      digitalWrite(col3Pin, HIGH);
      break;
  }
}
*/

void printDigit(int num, boolean dpFlag)
{
  byte data = digitArray[num];
  if (dpFlag == true)
    bitSet(data, 5);
  if (num > -1 && num < 10)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data);
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

void blip(int times, int d)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(alertPin, HIGH);
    delay(d);
    digitalWrite(alertPin, LOW);
    delay(d);
  }
}

void showWin()
{
  byte winAnim[] = {
    0b10000001,
    0b11000000,
    0b01000010,
    0b00000110,
    0b00001100,
    0b00011000,
    0b00010010,
    0b00000011
  };

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, winAnim[j]);
      digitalWrite(latchPin, 1);
      delay(90);
    }
  }
}

void clearLeds()
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
}
