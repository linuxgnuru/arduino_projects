/*

Example sketch 07

TEMPERATURE SENSOR

  Use the "serial monitor" window to read a temperature sensor.
  
  The TMP36 is an easy-to-use temperature sensor that outputs
  a voltage that's proportional to the ambient temperature.
  You can use it for all kinds of automation tasks where you'd
  like to know or control the temperature of something.
  
  More information on the sensor is available in the datasheet:
  http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Temp/TMP35_36_37.pdf

  Even more exciting, we'll start using the Arduino's serial port
  to send data back to your main computer! Up until now, we've 
  been limited to using simple LEDs for output. We'll see that
  the Arduino can also easily output all kinds of text and data.
  
Hardware connections:

  Be careful when installing the temperature sensor, as it is
  almost identical to the transistors! The one you want has 
  a triangle logo and "TMP" in very tiny letters. The
  ones you DON'T want will have "222" on them.

  When looking at the flat side of the temperature sensor
  with the pins down, from left to right the pins are:
  5V, SIGNAL, and GND.
  
  Connect the 5V pin to 5 Volts (5V).
  Connect the SIGNAL pin to ANALOG pin 0.
  Connect the GND pin to ground (GND).

This sketch was written by SparkFun Electronics,
with lots of help from the Arduino community.
This code is completely free for any use.
Visit http://www.arduino.cc to learn about the Arduino.

Version 2.0 6/2012 MDG
*/

const int temperaturePin = A0;

#if 0
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

typedef enum {
  CEL,
  FAR
} cf_enum;

// We'll use analog input 0 to measure the temperature sensor's signal pin.
const int f_cPin = 12;
const int alertPin = 13;
const int dataPin  = 2; // Pin connected to DS of 74HC595 - blue
const int latchPin = 3; // Pin connected to ST_CP of 74HC595 - green
const int clockPin = 4; // Pin connected to SH_CP of 74HC595 - yellow

int  buttonState; // current reading from the input pin
int  lastButtonState = LOW; // previous reading from the input pin
long lastDebounceTime = 0; // last time the output pin was toggled

const long debounceDelay = 50; // debounce time; increase if the output flickers

static int cfFlag = CEL;

byte data[] = { 
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};
#endif

void setup()
{
  /* In this sketch, we'll use the Arduino's serial port
     to send text back to the main computer. For both sides to
     communicate properly, they need to be set to the same speed.
     We use the Serial.begin() function to initialize the port
     and set the communications speed.
     The speed is measured in bits per second, also known as
     "baud rate". 9600 is a very commonly used baud rate,
     and will transfer about 10 characters per second. */
#if 0
  pinMode(alertPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(f_cPin, INPUT_PULLUP);
#endif
  Serial.begin(9600);
}

void loop()
{
#if 0
  int tens, ones, tenth, hundreth;
  float decTmp, dec, curTemp;
  int reading;
  reading = digitalRead(f_cPin);
  if (reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == LOW)
        cfFlag = !cfFlag;
    }
  }
  lastButtonState = reading;
#endif
  /* Up to now we've only used integer ("int") values in our
     sketches. Integers are always whole numbers (0, 1, 23, etc.).
     In this sketch, we'll use floating-point values ("float").
     Floats can be fractional numbers such as 1.42, 2523.43121, etc.
     We'll declare three floating-point variables
     (We can declare multiple variables of the same type on one line:) */
  float voltage, degreesC, degreesF;
/*   First we'll measure the voltage at the analog pin. Normally
     we'd use analogRead(), which returns a number from 0 to 1023.
     Here we've written a function (further down) called
     getVoltage() that returns the true voltage (0 to 5 Volts)
     present on an analog input pin. */
  voltage = getVoltage(temperaturePin);
  /* Now we'll convert the voltage to degrees Celsius.
     This formula comes from the temperature sensor datasheet: */
  degreesC = (voltage - 0.5) * 100.0;
  /* While we're at it, let's convert degrees Celsius to Fahrenheit.
     This is the classic C to F conversion formula: */

//  long a = analogRead(temperaturePin);  //read thermistor value 
//  degreesC = 4090 /(log((1025.0 * 10 / a - 10) / 10) + 4090 / 298.0) - 273.0;

  degreesF = degreesC * (9.0/5.0) + 32.0;
#if 0
  curTemp = (cfFlag == CEL ? degreesC : degreesF);
  curTemp = degreesF;
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
#endif
/*
   Now we'll use the serial port to print these values
   to the serial monitor!
   To open the serial monitor window, upload your code,
   then click the "magnifying glass" button at the right edge
   of the Arduino IDE toolbar. The serial monitor window
   will open.
   (NOTE: remember we said that the communication speed
   must be the same on both sides. Ensure that the baud rate
   control at the bottom of the window is set to 9600. If it
   isn't, change it to 9600.)
   Also note that every time you upload a new sketch to the
   Arduino, the serial monitor window will close. It does this
   because the serial port is also used to upload code!
   When the upload is complete, you can re-open the serial
   monitor window.
   To send data from the Arduino to the serial monitor window,
   we use the Serial.print() function. You can print variables
   or text (within quotes).>
*/
  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);
  /* These statements will print lines of data like this:
     "voltage: 0.73 deg C: 22.75 deg F: 72.96"
     Note that all of the above statements are "print", except
     for the last one, which is "println". "Print" will output
     text to the SAME LINE, similar to building a sentence
     out of words. "Println" will insert a "carriage return"
     character at the end of whatever it prints, moving down
     to the NEXT line. */   
  delay(1000); // repeat once per second (change as you wish!)
}

float getVoltage(int pin)
{
  /*
      This function has one input parameter, the analog pin number
       to read. You might notice that this function does not have
       "void" in front of it; this is because it returns a floating-
       point value, which is the true voltage on that pin (0 to 5V).
       You can write your own functions that take in parameters
       and return values. Here's how:
       To take in parameters, put their type and name in the
       parenthesis after the function name (see above). You can
       have multiple parameters, separated with commas.
       To return a value, put the type BEFORE the function name
       (see "float", above), and use a return() statement in your code
       to actually return the value (see below).
       If you don't need to get any parameters, you can just put
       "()" after the function name.
       If you don't need to return a value, just write "void" before
       the function name.
       Here's the return statement for this function. We're doing
       all the math we need to do within this statement:
*/
  return (analogRead(pin) * 0.004882814);
  /* This equation converts the 0 to 1023 value that analogRead()
     returns, into a 0.0 to 5.0 value that is the true voltage
     being read at that pin. */
}

/*
 Other things to try with this code:
   Turn on an LED if the temperature is above or below a value.
   Read that threshold value from a potentiometer - now you've
   created a thermostat!
 possibly combine both clears; and have a if flag
*/

#if 0
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

void printFloat(float num)
{
  float tmpT, tmpH;
  int tenth, hundth;

  if (num > 0 && num < 99)
  {
    printDigit(0, num / 10, false);
    printDigit(1, (int)num % 10, true);
    tmpT = num * 10;
    tenth = (int)tmpT % 10;
    printDigit(2, tenth, false);
    tmpH = num * 100;
    hundth = (int)tmpH % 10;
    printDigit(3, hundth, false);
  }
  else
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0b00000000);
    shiftOut(dataPin, clockPin, 0b00000000);
    shiftOut(dataPin, clockPin, 0b00000000);
    shiftOut(dataPin, clockPin, 0b00000000);
    digitalWrite(latchPin, 1);
  }
}

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
#endif

