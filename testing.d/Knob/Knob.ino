// Controlling a servo position using a potentiometer (variable resistor)
// by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = A0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

const int ledCount = 5;    // the number of LEDs in the bar graph

// an array of pin numbers to which LEDs are attached
const int ledPinsLeft[] = {
  6, 5, 4, 3, 2
};

// an array of pin numbers to which LEDs are attached
const int ledPinsRight[] = {
  7, 8, 9, 10, 12
};

void setup()
{
  myservo.attach(11);  // attaches the servo on pin 11 to the servo object
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    pinMode(ledPinsLeft[thisLed], OUTPUT);
    pinMode(ledPinsRight[thisLed], OUTPUT);
  }
  //  Serial.begin(9600);
}

void loop()
{
  int sensorReading = analogRead(potpin);
  int ledLevelL = map(sensorReading, 511, 0, 0, ledCount);
  int ledLevelR = map(sensorReading, 512, 1023, 0, ledCount);
  long cur_voltage = readVcc();
  // if voltage drops below 5001, turn on red led
  //  digitalWrite(1, (cur_voltage < 5001 ? HIGH : LOW));
  digitalWrite(A1, (cur_voltage < 4500));
  if (sensorReading < 511)
  {
    for (int i = 0; i < ledCount; i++)
      digitalWrite(ledPinsRight[i], LOW);
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
      digitalWrite(ledPinsLeft[thisLed], (thisLed < ledLevelL));
  }
  if (sensorReading > 511)
  {
    for (int i = 0; i < ledCount; i++)
      digitalWrite(ledPinsLeft[i], LOW);
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
      digitalWrite(ledPinsRight[thisLed], (thisLed < ledLevelR));
  }
  // If in the middle; turn on 13
  //  digitalWrite(LED_BUILTIN, (sensorReading >= 500 && sensorReading <= 520));
  digitalWrite(LED_BUILTIN, (digitalRead(6) == LOW && digitalRead(7) == LOW));
  /*
    int ledLevel = map(sensorReading, 0, 1023, 0, ledCount);
    for (int thisLed = 0; thisLed < ledCount; thisLed++)
      digitalWrite(ledPins[thisLed], (thisLed < ledLevel));
  */
  //#ifdef SERVO
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
  //#else
  //  delay(400);
  //#endif
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
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high << 8) | low;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

