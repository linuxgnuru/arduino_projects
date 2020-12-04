/*
  4 digit (via shift registers) temperature
 
 sample code:
 
 shiftOut(dataPin, clockPin, digitArray[k - 48]); // char k = '5';
 shiftOut(dataPin, clockPin, alphabetArray[c - 65]);  // char c = 'A' ... 'Z'
 
 */
#include <ctype.h>
#include <string.h>

#define DEBUG 1
#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor

typedef enum {
  CEL,
  FAR
} 
scale_enum;

boolean cfFlag;

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow wire

const int cfPin = 12;

int  buttonState; // current reading from the input pin
int  lastButtonState = LOW; // previous reading from the input pin
long lastDebounceTime = 0; // last time the output pin was toggled

const long debounceDelay = 50; // debounce time; increase if the output flickers

const byte alphabetArray[] = {
  0b11011011, // A
  0b11101001, // B
  0b01110001, // C
  0b11101011, // D
  0b11110001, // E
  0b11010001, // F
  0b01111001, // G
  0b11001011, // H
  0b01000001, // I
  0b01101000, // J
  0b11001111, // K (H.)
  0b01100001, // L
  0b01011111, // M (n.)
  0b01011011, // N (n)
  0b11101000, // O
  0b11010011, // P
  0b01111111, // Q (O.)
  0b11000000, // R (r)
  0b10111001, // S (5)
  0b10001000, // T
  0b01101011, // U
  0b01101000, // V
  0b01101100, // W
  0b01001011, // X
  0b10101011, // Y
  0b11110010, // Z (2)
  0b00000000 // space
};

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
#ifdef DEBUG
  Serial.begin(9600);
#endif
  //  cfFlag = CEL;
  cfFlag = FAR;
  pinMode(cfPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  printMessage("hello world", 250);
  /*
  for (int i = 0; i < 26; i++)
   {
   printChar(0, alphabetArray[i]);
   delay(500);
   }
   */
  //printDigit(0, 1, false);
  /*
  for (int i = 0; i < 4; i++)
   {
   for (int j = 0; j < 10; j++)
   {
   printDigit(i, j, false);
   delay(250);
   }
   }
   for (int i = 0; i < 4; i++)
   printDigit(i, 99, false);
   */
}

void loop()
{
  /*
#ifdef DEBUG
   if (Serial.available())
   {
   int val = Serial.read() - 48;
   printDigit(0, val, false);
   }
   #endif
   */
  //  getTemp();
  //  digitalWrite(LED_BUILTIN, (cfFlag == FAR ? HIGH : LOW));
  delay(1000);
}

void getTemp()
{
  float tempC, tempF;
  float curTemp;
  float decTmp, dec;
  int tens, ones, tenth, hundreth;
  int reading;
  float raw;
  tempC = (float)raw / 16.0;
  tempF = tempC * 1.8 + 32.0;
  reading = digitalRead(cfPin);
  if (reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
        cfFlag = (cfFlag == FAR ? CEL : FAR);
    }
  }
  lastButtonState = reading;
  curTemp = (cfFlag == CEL ? tempC : tempF);
  tens = (int)curTemp / 10;
  ones = (int)curTemp % 10;
  decTmp = (curTemp - ((float)tens * 10) - (float)ones); // for decimal
  dec = decTmp * 100;
  tenth = (int)dec / 10;
  hundreth = (int)dec % 10;
  data[0] = digitArray[tens];
  data[1] = digitArray[ones];
  bitSet(data[2], 2);
  data[2] = digitArray[tenth];
  data[3] = digitArray[hundreth];
  //  blink_dp(3, 250);
  printDigit(0, tens, false);
  printDigit(1, ones, true);
  printDigit(2, tenth, false);
  printDigit(3, hundreth, false);
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

// pos = 0 - 3
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

void blink_dp(int pos, int d)
{
  if (pos < 4)
  {
    byte myData = data[pos];
    bitSet(myData, 2);
    delay(d);
    bitClear(myData, 2);
  }
}

void printMessage(char *message, int d)
{
  byte tb;
  byte b[4];
  int pos = 3;
  int ctr = 0;

//  Serial.println(message);  Serial.print("b: ");
  for (int i = 0; i < strlen(message); i++)
  {
    clearD(pos);
    if (message[i] == 32)
     tb = 0b00000000;
    else
      tb = alphabetArray[toupper(message[i]) - 65];
    data[pos--] = tb;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    digitalWrite(latchPin, 1);
    delay(d);
    if (pos == -1)
      pos = 3;
    data[pos] = tb;
    ctr = 0;
  }
}

void printChar(int pos, char k)
{
  int ctr = 0;

  if (pos < 4)
  {
    k = toupper(k);
    data[pos] = alphabetArray[k - 65];
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    shiftOut(dataPin, clockPin, data[ctr++]);
    digitalWrite(latchPin, 1);
  }
}

void clearD(int pos)
{
  int ctr = 0;
  
  data[pos] = 0b00000000;
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  digitalWrite(latchPin, 1);
}

