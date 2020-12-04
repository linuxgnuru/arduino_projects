/*
   PS2 controller / joypad
 
          +-------+
   GND ---+O     O|
   +5v ---+       |
   VRx ---+       |
   VRy ---+O     O|
    SW    +-------+

servo:
 Brown - ground
   Red - 5V
Yellow - pin

*/

#include <Servo.h> 

Servo myservoUD;  // create servo object to control a servo 
Servo myservoLR;  // create servo object to control a servo 

int servoPinUD =  9; // vertical
int servoPinLR = 10; // horizontal
int xPin = A0;  // analog pin for joystick 
int yPin = A1;  // 
int swPin = 8; // button on joystick
int xVal;    // variable to read the value from the analog pin 
int yVal;    //
int i;
//#define DEBUG 1
 
void setup() 
{ 
  myservoUD.attach(servoPinUD);  // attaches the servo on pin 9 to the servo object 
  myservoLR.attach(servoPinLR);  // attaches the servo on pin 9 to the servo object 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(swPin, INPUT_PULLUP);
#ifdef DEBUG
  Serial.begin(9600);
#endif
}
 
void loop()
{ 
  int xReading = analogRead(xPin);
  int yReading = analogRead(yPin);
#ifdef DEBUG
  Serial.print("x, y: ");
  Serial.print(xReading);
  Serial.print(", ");
  Serial.println(yReading);
#endif
  int servoDegreeY = map(yReading, 0, 1023, 0, 179);
  int servoDegreeX = map(xReading, 0, 1023, 0, 179);
  myservoUD.write(servoDegreeY);
  myservoLR.write(servoDegreeX);
  digitalWrite(13, (digitalRead(swPin) == LOW ? HIGH : LOW));
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
  digitalWrite(13, (digitalRead(6) == LOW && digitalRead(7) == LOW ? HIGH : LOW));
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 179, 0);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  delay(15);                           // waits for the servo to get there 
}

long readVcc()
{
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
  long result = (high<<8) | low;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
*/
