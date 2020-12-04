#include <Wire.h>
#include <RTClib.h>
#include <OneWire.h>

RTC_PCF8563 RTC;

#include <ctype.h>
#include <string.h>

//#define ANALOG_SENSOR 1
#define DS_PIN 10
//#define DEBUG 1
#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor

OneWire ds(DS_PIN);

typedef enum {
  TIME_F,
  TEMP_F
} flags_enum;

const int ttPin = 7;
const int calPin = 12;
const int secPin = 11;

static int tt = TIME_F;

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow wire

int  buttonState; // current reading from the input pin
int  lastButtonState = LOW; // previous reading from the input pin
long lastDebounceTime = 0; // last time the output pin was toggled

int  buttonStateS; // current reading from the input pin
int  lastButtonStateS = LOW; // previous reading from the input pin
long lastDebounceTimeS = 0; // last time the output pin was toggled

static boolean secFlag = false;

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
//  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(ttPin, INPUT);
  pinMode(calPin, INPUT_PULLUP);
  pinMode(secPin, INPUT_PULLUP);
  Wire.begin();
  RTC.begin();
  // following line sets the RTC to the date & time this sketch was compiled
  if (digitalRead(calPin) == LOW)
    RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
  int h, m, s;
  boolean pm = false;
  int reading;
  int readingS;

  DateTime now = RTC.now();
  h = now.hour();
  if (h >= 12)
      pm = true;
  if (h > 12)
    h -= 12;
  m = now.minute();
  s = now.second();
  readingS = digitalRead(secPin);
  if (readingS != lastButtonStateS)
    lastDebounceTimeS = millis();
  if ((millis() - lastDebounceTimeS) > debounceDelay)
  {
    if (readingS != buttonStateS)
    {
      buttonStateS = readingS;
      if (buttonStateS == LOW)
        secFlag = !secFlag;
    }
  }
  lastButtonStateS = readingS;
  reading = digitalRead(ttPin);
  if (reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
        tt = (tt == TIME_F ? TEMP_F : TIME_F);
    }
  }
  lastButtonState = reading;
  if (tt == TIME_F)
  {
    if (secFlag == false)
    {
      printDigit(0, h / 10, false);
      printDigit(1, h % 10, true);
      printDigit(2, m / 10, false);
      printDigit(3, m % 10, (pm == true ? true : false));
    }
    else
    {
//      for (int j = 0; j < 4; j++)        clearD(j);
      printDigit(0, m / 10, false);
      printDigit(1, m % 10, true);
      printDigit(2, s / 10, false);
      printDigit(3, s % 10, false);
    }
  }
  else
    getTemp();
  delay(250);
}

void getTemp()
{
  float curTemp;
  float tempF, tempC;
  float decTmp, dec;
  int tens, ones, tenth, hundreth;
  float tmp;
#ifndef ANALOG_SENSOR
  byte i;
  byte present = 0;
  byte type_s = 0;
  byte data[12];
  byte addr[8];
#endif
  
#ifdef ANALOG_SENSOR
  long a = analogRead(analogPin);  //read thermistor value 
  //the calculating formula of temperature
  tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
#else
  if (!ds.search(addr))
  {
    ds.reset_search();
    delay(250);
    return;
  }
  if (OneWire::crc8(addr, 7) != addr[7])
      return;
  switch (addr[0])
  {
    case 0x10:      type_s = 1;      break;
    case 0x28:      type_s = 0;      break;
    case 0x22:      type_s = 0;      break;
    default:      return;
  } 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  delay(1000); // maybe 750ms is enough, maybe not
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  for ( i = 0; i < 9; i++) // we need 9 bytes
    data[i] = ds.read();
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s)
  {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10)
      raw = (raw & 0xFFF0) + 12 - data[6]; // "count remain" gives full 12 bit resolution
  }
  else
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7;      // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  tempC = (float)raw / 16.0;
#endif
  tempF = tempC * 1.8 + 32.0;
  curTemp = tempF;
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
  printDigit(0, tens, false);
  printDigit(1, ones, true);
  printDigit(2, tenth, false);
  printDigit(3, hundreth, false);
}

// Get the internal temperature of the Arduino
double GetTempInt(void)
{
  unsigned int wADC;
  double t;
  double u;
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN); // enable the ADC
  delay(20); // wait for voltages to become stable
  ADCSRA |= _BV(ADSC); // start the ADC
  while (bit_is_set(ADCSRA, ADSC));
  wADC = ADCW;
  t = (wADC - 324.31) / 1.22;
  u = ((t * 9) / 5) + 32;
  t = u;
  return (t);
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

