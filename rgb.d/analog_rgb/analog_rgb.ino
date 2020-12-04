/*
 * PS2 controller / joypad
 
          +-------+
   GND ---+O     O|
   +5v ---+       |
   VRx ---+       |
   VRy ---+O     O|
    SW    +-------+

*/

const int rPin = 11;
const int gPin = 10;
const int bPin = 9;
const int aPin = A0; // any run of the mill analog input
const int xPin = A1; // analog pin for joystick
const int yPin = A2;
int i;
 
void setup() 
{ 
  pinMode(LED_BUILTIN, OUTPUT);
//  pinMode(swPin, INPUT_PULLUP);
//  Serial.begin(9600);
}
 
void loop()
{ 
//  Serial.println(analogRead(aPin));
  analogWrite(rPin, map(analogRead(aPin), 0, 1023, 0, 255));
  analogWrite(gPin, map(analogRead(xPin), 0, 1023, 0, 255));
  analogWrite(bPin, map(analogRead(yPin), 0, 1023, 0, 255));
  delay(50);
}

//  if (digitalRead(swPin) == HIGH)
 /* 
  int ledLevelL = map(sensorReading, 511, 0, 0, ledCount);
  int ledLevelR = map(sensorReading, 512, 1023, 0, ledCount);
//  long cur_voltage = readVcc();
  // if voltage drops below 5001, turn on red led
//  digitalWrite(1, (cur_voltage < 5001 ? HIGH : LOW));
//  digitalWrite(1, (cur_voltage < 4500 ? HIGH : LOW));
  if (sensorReading < 511)
  {
    for (i = 0; i < ledCount; i++) digitalWrite(ledPinsRight[i], LOW);
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
      digitalWrite(ledPinsLeft[thisLed], (thisLed < ledLevelL ? HIGH : LOW));
  }
  if (sensorReading > 511)
  {
    for (i = 0; i < ledCount; i++) digitalWrite(ledPinsLeft[i], LOW);
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
      digitalWrite(ledPinsRight[thisLed], (thisLed < ledLevelR ? HIGH : LOW));
  }
  // If in the middle; turn on 13
  digitalWrite(LED_BUILTIN, (digitalRead(6) == LOW && digitalRead(7) == LOW ? HIGH : LOW));
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 179, 0);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
}
const int rPinA = 11;
const int gPinA = 10;
const int bPinA = 9;

const int rPinB = 6;
const int gPinB = 5;
const int bPinB = 3;

void setup()
{
  pinMode(rPinA, OUTPUT);
  pinMode(gPinA, OUTPUT);
  pinMode(bPinA, OUTPUT);
  pinMode(rPinB, OUTPUT);
  pinMode(gPinB, OUTPUT);
  pinMode(bPinB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  unsigned int rgbA[3];
  unsigned int rgbB[3];
  int rev_dec;

  rgbA[0] = rgbB[2] = 255;
  rgbA[1] = rgbA[2] = rgbB[0] = rgbB[1] = 0;
  for (int decrement = 0; decrement < 3; decrement++)
  {
    rev_dec = abs(decrement - 2);
    int incrementA = decrement == 2 ? 0 : decrement + 1;
    int incrementB = rev_dec == 2 ? 0 : rev_dec + 1;
    for (int i = 0; i < 255; i += 1)
    {
      rgbA[decrement] -= 1;
      rgbA[incrementA] += 1;
      color(rPinA, gPinA, bPinA, rgbA[0], rgbA[1], rgbA[2]);
      rgbB[rev_dec] -= 1;
      rgbB[incrementB] += 1;
      color(rPinB, gPinB, bPinB, rgbB[0], rgbB[1], rgbB[2]);
      delay(5);
    }
  }
}

void color(int rPin, int bPin, int gPin, unsigned char red, unsigned char green, unsigned char blue)
{
  analogWrite(rPin, red);
  analogWrite(bPin, blue);
  analogWrite(gPin, green);
}
*/
