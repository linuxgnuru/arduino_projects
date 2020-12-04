/* 
 6-13-2011
 Spark Fun Electronics 2011
 Nathan Seidle

 This code is public domain but you buy me a beer if you use this and we meet 
 someday (Beerware license).

 3 digit 7 segment display:
 http://www.sparkfun.com/products/9483
 Datasheet: 
 http://www.sparkfun.com/datasheets/Components/LED/7-Segment/YSD-439AR6B-35.pdf

 This is an example of how to drive a 7 segment LED display from an ATmega
 without the use of current limiting resistors. This technique is very common 
 but requires some knowledge of electronics - you do run the risk of dumping 
 too much current through the segments and burning out parts of the display. 
 If you use the stock code you should be ok, but be careful editing the 
 brightness values.

 This code should work with all colors (red, blue, yellow, green) but the 
 brightness will vary from one color to the next because the forward voltage 
 drop of each color is different. This code was written and calibrated for the 
 red color.

 This code will work with most Arduinos but you may want to re-route some of 
 the pins.

 8 segments
 3 digits
 =
 10 pins required for full control 

 */

#define beta 4090 //the beta of the thermistor

typedef enum {
  CEL,
  FAR
} scale_enum;

const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue wire
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green wire
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow wire

const int cfPin = 12; // cel or far pin

int digit1 = 11; // PWM Display pin 1
int digit2 = 10; // PWM Display pin 2
int digit3 =  9; // PWM Display pin 6

const int analogPin = A0; // the thermistor attach to 

boolean cfBool; // cel or far bool

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
  pinMode(cfPin, INPUT_PULLUP);
  pinMode(latchPin, OUTPUT);
  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  displayNumber(getTemp());
  delayMicroseconds(500);
  // long startTime = millis();
//  displayNumber(millis() / 1000);
/*
  while ((millis() - startTime) < 2000)
  {
    displayNumber(127);
  }
  delay(1000);  
*/
}

/*
 Given a number, we display 327
 After running through the 3 numbers, the display is left turned off

 Display brightness
 Each digit is on for a certain amount of microseconds
 Then it is off until we have reached a total of 20ms for the function call
 Let's assume each digit is on for 1000us
 Each digit is on for 1ms, there are 3 digits, so the display is off for 16ms.
 That's a ratio of 1ms to 16ms or 6.25% on time (PWM).
 Let's define a variable called brightness that varies from:
 5000 blindingly bright (15.7mA current draw per digit)
 2000 shockingly bright (11.4mA current draw per digit)
 1000 pretty bright (5.9mA)
 500 normal (3mA)
 200 dim but readable (1.4mA)
 50 dim but readable (0.56mA)
 5 dim but readable (0.31mA)
 1 dim but readable in dark (0.28mA)
*/

void displayNumber(int toDisplay)
{
#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  LOW
#define DIGIT_OFF  HIGH

  long beginTime = millis();

  for (int digit = 3; digit > 0; digit--)
  {
    // Turn on a digit for a short amount of time
    switch (digit)
    {
      case 1: digitalWrite(digit1, DIGIT_ON); break;
      case 2: digitalWrite(digit2, DIGIT_ON); break;
      case 3: digitalWrite(digit3, DIGIT_ON); break;
    }
    // Turn on the right segments for this digit
    lightNumber(toDisplay % 10, (digit == 2 ? true : false));
    toDisplay /= 10;
    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    // Display digit for fraction of a second (1us to 5000us, 500 is pretty good)
    // Turn off all segments
    lightNumber(10, false); 
    // Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
  }
  // Wait for 20ms to pass before we paint the display again
  while ((millis() - beginTime) < 10)
    ;
}

// Given a number, turns on those segments
// If number == 10, then turn off number
void lightNumber(int numberToDisplay, boolean decimal)
{
  data = (numberToDisplay == 10 ? 0 : digitArray[numberToDisplay]);
  if (numberToDisplay != 10 && decimal == true)
    bitSet(data, 5);
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data);
  digitalWrite(latchPin, 1);
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

int getTemp()
{
  float curTemp;
  float tmp;
//  int tens, ones, decp;

  //read thermistor value 
  long a = analogRead(analogPin);

  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = ((tempC * 9) / 5) + 32;
  cfBool = (digitalRead(cfPin) == HIGH ? FAR : CEL);
  curTemp = (cfBool == CEL ? tempC : tempF);
  tmp = curTemp * 10;
  return (int)tmp;
}
  /*
  tens = (int)curTemp / 10;
  ones = (int)curTemp % 10;
  tmp = curTemp * 10;
  decp = (int)tmp % 10;
#ifdef DEBUG
Serial.print("curTemp: ");
Serial.print(curTemp);
Serial.print(" ");
Serial.print(tens);
Serial.print(" ");
Serial.print(ones);
Serial.print(" . ");
Serial.println(decp);
#endif
    last = (int)tempF % 100;
    dec = tempF - (float)first; // for decimal
    dec *= 100;
    dec *= 10;
    last = (int)dec;

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
*/

