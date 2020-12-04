/*
  4 digit (via shift registers) temperature
*/
#include <OneWire.h>

//#define DEBUG 1
#define DS_PIN 10
#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor

OneWire ds(DS_PIN);

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
//  BCDGE.FA
  0b01110111, // A
  0b00011111, // B
  0b00101101, // C
  0b01011110, // D
  0b00101111, // E
  0b00100111, // F
  0b00111101, // G
  0b01010111, // H
  0b01010000, // I
  0b01011100, // J
  0b11010111, // K (H.)
  0b00001101, // L
  0b10010110, // M (n.) c b e f
  0b00010110, // N (n)  c b e
  0b01111101, // O
  0b01100111, // P
  0b11111101, // Q (O.)
  0b00000110, // R (r)
  0b00111011, // S
  0b01110000, // T
  0b01011101, // U
  0b00011100, // V
  0b00111100, // W
  0b01010101, // X
  0b01011011, // Y
  0b01101110, // Z (2)
  0b00000000 // space
};

/*
   ---     G
  |   |  A   F
   ---     B
  |   |  C   E
   ---     D   H (dp)
*/
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
  0b00000000, 0b00000000, 0b00000000, 0b00000000 };

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
//#if 0
  int reading;
  int ctr;

#ifdef DEBUG
  if (Serial.available())
  {
    int val = Serial.read() - 48;
    printDigit(0, val, false);
  }
#endif
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
  digitalWrite(LED_BUILTIN, (cfFlag == FAR ? HIGH : LOW));
  getTemp();
/*  
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  shiftOut(dataPin, clockPin, data[ctr++]);
  digitalWrite(latchPin, 1);
  */
//  delay(5000);
  delay(1000);
}

void getTemp()
{
  byte i;
  byte present = 0;
  byte type_s = 0;
  byte data[12];
  byte addr[8];
  float tempC, tempF;
  float curTemp;
  float decTmp, dec;
  int tens, ones, tenth, hundreth;

  if (!ds.search(addr))
  {
    ds.reset_search();
    delay(250);
    return;
  }
  if (OneWire::crc8(addr, 7) != addr[7])
  {
//      Serial.println("CRC is not valid!");
      return;
  }
  switch (addr[0])
  {
    case 0x10:
//      Serial.println("  Chip = DS18S20");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
//      Serial.println("  Chip = DS18B20");
      type_s = 0;
      break;
    case 0x22:
//      Serial.println("  Chip = DS1822");
      type_s = 0;
      break;
    default:
//      Serial.println("Device is not a DS18x20 family device.");
      return;
  } 
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  delay(1000); // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  for ( i = 0; i < 9; i++) // we need 9 bytes
  {
    data[i] = ds.read();
//    Serial.print(data[i], HEX);    Serial.print(" ");
  }
  /*
   Convert the data to actual temperature
   because the result is a 16 bit signed integer, it should
   be stored to an "int16_t" type, which is always 16 bits
   even when compiled on a 32 bit processor.
   */
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
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;      // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    // default is 12 bit resolution, 750 ms conversion time
  }
  tempC = (float)raw / 16.0;
  tempF = tempC * 1.8 + 32.0;
  curTemp = (cfFlag == CEL ? tempC : tempF);
  tens = (int)curTemp / 10;
  ones = (int)curTemp % 10;
  decTmp = (curTemp - ((float)tens * 10) - (float)ones); // for decimal
//  dec = decTmp * 10;
  dec = decTmp * 100;
  tenth = (int)dec / 10;
  hundreth = (int)dec % 10;
//Serial.print("decTmp * 10: "); Serial.println(dec);
//  tenth = (int)dec;  dec = decTmp * 100;
//Serial.print("decTmp * 100: "); Serial.println(dec);
//  hundreth = (int)dec;  hundreth /= 10;
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
  Serial.print("curTemp: ");
  Serial.println(curTemp);
//#endif
//#if 0
  Serial.print("decTmp: ");
  Serial.println(decTmp);
  Serial.print("[");
  Serial.print(tens);
  Serial.print("] [");
  Serial.print(ones);
  Serial.print("] . [");
  Serial.print(tenth);
  Serial.print("] [");
  Serial.print(hundreth);
  Serial.println("]");
#endif
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
//  byte myData[4];

  if (num > -1 && num < 10 || num == 99)
  {
//    for (int i = 0; i < 4; i++)      myData[i] = data[i];
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

