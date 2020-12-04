 
int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = LED_BUILTIN;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(2, OUTPUT);  
  pinMode(7, OUTPUT);
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  // read the value from the sensor:
  long vcc = readVcc();
  sensorValue = analogRead(sensorPin);    
  int scale = map(sensorValue, 0, 1023, 0, 15);
#ifdef DEBUG
  Serial.print(scale);
  Serial.print(" ");
  Serial.println(vcc);
#endif
  digitalWrite(7, (sensorValue < 1023 ? HIGH : LOW));
  digitalWrite(2, (vcc < 5001 ? HIGH : LOW));
  delay(400);
  // turn the ledPin on
//  digitalWrite(ledPin, HIGH);  
  // stop the program for <sensorValue> milliseconds:
//  delay(sensorValue);          
  // turn the ledPin off:        
//  digitalWrite(ledPin, LOW);   
  // stop the program for for <sensorValue> milliseconds:
//  delay(sensorValue);                  
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
