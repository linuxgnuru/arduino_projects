#include <Wire.h>

#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor

#define SLAVE_ADDRESS 0x04
int tmp_reading;
int number = 0;
int state = 0;
int led_state;
int ldr;
int potA;
int potB;

long a;
double temp;
float tempC;
float tempF;

int ledPinG = 9;
int ledPinY = 10;
int ledPinR = 11;

int onG = 0;
int onY = 0;
int onR = 0;

/*
 * Buttons and debouncing
 */
int butPinG = 5;
int butPinY = 6;
int butPinR = 7;

// variables which change
int ledStateG = LOW; // current state of the output pin
int ledStateY = LOW; // current state of the output pin
int ledStateR = LOW; // current state of the output pin
int buttonStateG;     // current reading from the input pin
int buttonStateY;     // current reading from the input pin
int buttonStateR;     // current reading from the input pin
int lastButtonStateG = LOW; // previous reading from the input pin
int lastButtonStateY = LOW; // previous reading from the input pin
int lastButtonStateR = LOW; // previous reading from the input pin

long lastDebounceTimeG = 0; // last time the output pin was toggled
long lastDebounceTimeY = 0; // last time the output pin was toggled
long lastDebounceTimeR = 0; // last time the output pin was toggled

long debounceDelay = 50; // debounce time; increase if the output flickers

void setup()
{
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinY, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  pinMode(butPinG, INPUT);
  pinMode(butPinY, INPUT);
  pinMode(butPinR, INPUT);
  digitalWrite(ledPinG, ledStateG);
  digitalWrite(ledPinY, ledStateY);
  digitalWrite(ledPinR, ledStateR);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}

void loop()
{
  delay(100);
  // Green button
  tmp_reading = digitalRead(butPinG);
  if (tmp_reading != lastButtonStateG) lastDebounceTimeG = millis();
  if ((millis() - lastDebounceTimeG) > debounceDelay)
  {
    if (tmp_reading != buttonStateG)
    {
      buttonStateG = tmp_reading;
      if (buttonStateG == HIGH) ledStateG = !ledStateG;
    }
  }
  digitalWrite(ledPinG, ledStateG);
  lastButtonStateG = tmp_reading;
  // Yellow button
  tmp_reading = digitalRead(butPinY);
  if (tmp_reading != lastButtonStateY) lastDebounceTimeY = millis();
  if ((millis() - lastDebounceTimeY) > debounceDelay)
  {
    if (tmp_reading != buttonStateY)
    {
      buttonStateY = tmp_reading;
      if (buttonStateY == HIGH) ledStateY = !ledStateY;
    }
  }
  digitalWrite(ledPinY, ledStateY);
  lastButtonStateY = tmp_reading;
  // Red button
  tmp_reading = digitalRead(butPinR);
  if (tmp_reading != lastButtonStateR) lastDebounceTimeR = millis();
  if ((millis() - lastDebounceTimeR) > debounceDelay)
  {
    if (tmp_reading != buttonStateR)
    {
      buttonStateR = tmp_reading;
      if (buttonStateR == HIGH) ledStateR = !ledStateR;
    }
  }
  digitalWrite(ledPinR, ledStateR);
  lastButtonStateR = tmp_reading;
  temp = GetTemp();
}

void receiveData(int byteCount)
{
  while (Wire.available())
  {
    number = Wire.read();
    switch (number)
    {
      case 1:
        onG = (digitalRead(ledPinG) == HIGH ? 1 : 0);
        onY = (digitalRead(ledPinY) == HIGH ? 1 : 0);
        onR = (digitalRead(ledPinR) == HIGH ? 1 : 0);
        if (onG == 0 && onY == 0 && onR == 0) led_state = 0;
        if (onG == 0 && onY == 0 && onR == 1) led_state = 1;
        if (onG == 0 && onY == 1 && onR == 0) led_state = 10;
        if (onG == 0 && onY == 1 && onR == 1) led_state = 11;
        if (onG == 1 && onY == 0 && onR == 0) led_state = 100;
        if (onG == 1 && onY == 0 && onR == 1) led_state = 101;
        if (onG == 1 && onY == 1 && onR == 0) led_state = 110;
        if (onG == 1 && onY == 1 && onR == 1) led_state = 111;
        number = led_state;
        break;
      case 2:
        number = (int)temp;
        break;
      case 3:
        ldr = analogRead(A1);
        number = ldr;
        break;
      case 4:
          a = analogRead(A0);
          //the calculating formula of temperature
          tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
          tempF = ((tempC * 9) / 5) + 32;
          number = (int)tempF;
          break;
      case 5:
        potA = analogRead(A2);
        number = potA;
        break;
      case 6:
        potB = analogRead(A3);
        number = potB;
        break;
      default:
        break;
    }
  }
}

void sendData()
{
  Wire.write(number);
}

// Get the internal temperature of the Arduino
double GetTemp(void)
{
  unsigned int wADC;
  double t;
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN); // enable the ADC
  delay(20); // wait for voltages to become stable
  ADCSRA |= _BV(ADSC); // start the ADC
  while (bit_is_set(ADCSRA, ADSC));
  wADC = ADCW;
  t = (wADC - 324.31) / 1.22;
  return (t);
}
