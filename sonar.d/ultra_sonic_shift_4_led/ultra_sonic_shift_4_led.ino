/*
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
 Ultrasonic sensor Pins:
 VCC: +5VDC
 Trig : Trigger (INPUT) - Pin11
 Echo: Echo (OUTPUT) - Pin 12
 GND: GND
 */
#include <ctype.h>
#include <string.h>

//#define DEBUG 1

const int dataPin  = 4; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 2; // Pin connected to SH_CP of 74HC595 - yellow wire

const int trigPin = 11;    // Trig - Yellow Jumper
const int echoPin = 12;    // Echo - Orange Jumper

const boolean cmFlag = true; // cm == true -> cm, else -> inches

long duration;
int cm, inches;

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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
}

void loop()
{
  float cm, in;
  float decTmp, dec;
  int tens, ones, tenth, hundreth;
  float num;
  cm = sonar();
  in = (cm * 0.393701);
  //  in = (duration / 2) / 74; // 37
  num = (cmFlag == true ? cm : in);
  if (cm != 0 && cm < 100)
  {
    tens = (int)num / 10;
    ones = (int)num % 10;
    decTmp = (num - ((float)tens * 10) - (float)ones); // for decimal
    dec = decTmp * 100;
    tenth = (int)dec / 10;
    hundreth = (int)dec % 10;
    data[0] = digitArray[tens];
    data[1] = digitArray[ones];
    bitSet(data[2], 2);
    data[2] = digitArray[tenth];
    data[3] = digitArray[hundreth];
    printDigit(0, tens, false);
    printDigit(1, ones, true);
    printDigit(2, tenth, false);
    printDigit(3, hundreth, false);
#ifdef DEBUG
    Serial.print("cm: ");
    Serial.print(cm);
    Serial.print(" in: ");
    Serial.println(in);
#endif
  }
  /*
  printDigit(0, sonar(true), false);
   printDigit(1, sonar(true) / 10, false);
   printDigit(1, h % 10, true);
   printDigit(2, m / 10, false);
   printDigit(3, m % 10, (pm == true ? true : false));
   */
  delay(1000);
}

float sonar()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  //  Serial.print(" duration: ");  Serial.println(duration);
  // convert the time into a distance
  //  cm = duration / 58; //(duration/2) / 29.1;
  return ((duration / 2) / 29.1);
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


