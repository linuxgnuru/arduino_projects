/*
 * 7 segment LED + shift register (74HC595) + temperature sensor + 4 LEDs for tens (6, 7, 8, 9)
 */
#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
#define CEL 1
#define FAR 2

int cel_far_flag;
int i;

// Debounce
int buttonState; // current reading from the input pin
int lastButtonState = LOW; // previous reading from the input pin
long lastDebounceTime = 0; // last time the output pin was toggled

long debounceDelay = 50; // debounce time; increase if the output flickers

/*
  --    A
 |  | B   G
  --    C
 |  | D   F  
  --    E

  --    A
    |     G
  --    C
 |    D   
  --    E
 */

int ledPinA = LED_BUILTIN; // 13
int ledPinB = 9;
int ledPinC = 7;
int ledPinD = 6;
int ledPinE = 5;
int ledPinF = 3;
int ledPinG = 4;

//Pin connected to ST_CP of 74HC595
int latchPin = 8; // green
//Pin connected to SH_CP of 74HC595
int clockPin = 12; // yellow
//Pin connected to DS of 74HC595
int dataPin = 11; // blue
const byte inputPin = 10;

/*
 0 = abdefg   = 0b11011101
 1 = dg       = 0b01010000
 2 = adcef    = 0b11001110
 3 = adcgf    = 0b11011010
 4 = bcdg     = 0b01010011
 5 = abcgf    = 0b10011011
 6 = abcefg   = 0b10011111
 7 = adg      = 0b11010000
 8 = abcdefg  = 0b11011111
 9 = abcdfg   = 0b11011011
 */

byte digitArray[] = {
  0b11011101,
  0b01010000,
  0b11001110,
  0b11011010,
  0b01010011,
  0b10011011,
  0b10011111,
  0b11010000,
  0b11011111,
  0b11011011
};

byte data;

void setup()
{
  cel_far_flag = FAR;
  pinMode(latchPin, OUTPUT);
  pinMode(inputPin, INPUT_PULLUP);
  for (i = 2; i < 8; i++)
    pinMode(i, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
/*
  for (i = 0; i < 10; i++)
  {
    ledDigit(i);
    delay(500);
  }
  */
  //  Serial.begin(9600);
  blinkAll_2Bytes(4, 200);
}

void loop()
{
  int tmp_reading;
  // Green button
  tmp_reading = digitalRead(inputPin);
  if (tmp_reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (tmp_reading != buttonState)
    {
      buttonState = tmp_reading;
      if (buttonState == HIGH)
        cel_far_flag = (cel_far_flag == CEL ? FAR : CEL);
    }
  }
  lastButtonState = tmp_reading;
  //read thermistor value 
  long a = analogRead(analogPin);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = ((tempC * 9) / 5) + 32;
  float cur_temp;
  int first;
  int last;
  cur_temp = (cel_far_flag == CEL ? tempC : tempF);
  first = (int)cur_temp / 10;
  last = (int)cur_temp % 10;
  //  Serial.println(tempF);
  //  last = (int)tempF % 100;
  //  dec = tempF - (float)first; // for decimal
  //dec *= 100;
  //  dec *= 10;
  //  last = (int)dec;
  ledDigit(first);
  data = digitArray[last];
  digitalWrite(latchPin, 0);
  //move 'em out
  shiftOut(dataPin, clockPin, data);
  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latchPin, 1);
  delay(1000);
  //  }
}

// my own creation
void ledDigit(int digit)
{
  //  clearDigit();
  switch (digit)
  {
  case 0:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinC, LOW);
    digitalWrite(ledPinD, HIGH);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  case 1:
    digitalWrite(ledPinA, LOW);
    digitalWrite(ledPinB, LOW);
    digitalWrite(ledPinC, LOW);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, LOW);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  case 2:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, LOW);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, HIGH);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, LOW);
    digitalWrite(ledPinG, HIGH);
    break;
  case 3:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, LOW);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  case 4:
    digitalWrite(ledPinA, LOW);
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, LOW);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  case 5:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, LOW);
    break;
  case 6:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, HIGH);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, LOW);
    break;
  case 7:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, LOW);
    digitalWrite(ledPinC, LOW);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, LOW);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  case 8:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, HIGH);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  case 9:
    digitalWrite(ledPinA, HIGH);
    digitalWrite(ledPinB, HIGH);
    digitalWrite(ledPinC, HIGH);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, HIGH);
    digitalWrite(ledPinF, HIGH);
    digitalWrite(ledPinG, HIGH);
    break;
  default:
    digitalWrite(ledPinA, LOW);
    digitalWrite(ledPinB, LOW);
    digitalWrite(ledPinC, LOW);
    digitalWrite(ledPinD, LOW);
    digitalWrite(ledPinE, LOW);
    digitalWrite(ledPinF, LOW);
    digitalWrite(ledPinG, LOW);
    break;
  }
}

void clearDigit()
{
  digitalWrite(10, LOW);
  digitalWrite(9, LOW);
  for (i = 3; i < 8; i++)
    digitalWrite(i, LOW);
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low
  //internal function setup
  int i = 0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }
  //stop shifting
  digitalWrite(myClockPin, 0);
}

//blinks the whole register based on the number of times you want to
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d)
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(50);
  /*
  for (int z = 0; z < 10; z++)
   {
   ledDigit(z);
   delay(200);
   }
   */
  for (int x = 0; x < n; x++)
  {
    //    for (i = 2; i < 6; i++)      digitalWrite(i, HIGH);
    ledDigit(8);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    //    for (i = 2; i < 6; i++)      digitalWrite(i, LOW);
    ledDigit(10);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}




/*
  {
 for (int k = 0; k < 4; k++)
 {
 switch (k)
 {
 case 0:
 digitalWrite(ledPin60, HIGH);
 digitalWrite(ledPin70, LOW);
 digitalWrite(ledPin80, LOW);
 digitalWrite(ledPin90, LOW);
 break;
 case 1:
 digitalWrite(ledPin60, LOW);
 digitalWrite(ledPin70, HIGH);
 digitalWrite(ledPin80, LOW);
 digitalWrite(ledPin90, LOW);
 break;
 case 2:
 digitalWrite(ledPin60, LOW);
 digitalWrite(ledPin70, LOW);
 digitalWrite(ledPin80, HIGH);
 digitalWrite(ledPin90, LOW);
 break;
 case 3:
 digitalWrite(ledPin60, LOW);
 digitalWrite(ledPin70, LOW);
 digitalWrite(ledPin80, LOW);
 digitalWrite(ledPin90, HIGH);
 break;
 }
 }
 for (int k = 1; k > -1; k--)
 {
 ledDigit(k);
 for (int j = 9; j > -1; j--)
 {
 //load the light sequence you want from array
 data = digitArray[j];
 //ground latchPin and hold low for as long as you are transmitting
 digitalWrite(latchPin, 0);
 //move 'em out
 shiftOut(dataPin, clockPin, data);
 //return the latch pin high to signal chip that it
 //no longer needs to listen for information
 digitalWrite(latchPin, 1);
 delay(500);
 }
 }
 }
 else
 {
 */



