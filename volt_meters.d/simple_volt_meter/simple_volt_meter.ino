/*
  4 digit (via shift registers) volt meter

  ---     A
  |   |  F   B
  ---     G
  |   |  E   C
  ---     D    dp

    analog part:
    3.3-30V in ---> 100K ohm /|\   ------> 10K ohm ---> Gnd
                              |
                              |
                           Analog 0
*/

#define NUM_SAMPLES 10

//#define DEBUG 1

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage

#define DL 100

void setup()
{
  //Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  //int tens, ones, tmp, decpA, decpB;
  float myvin;
  float pvin;
  /*
  tens = (int)myvin / 10;
  ones = (int)myvin % 10;
  tmp = myvin * 10;
  decpA = (int)tmp % 10;
  decpB = (int)tmp % 100;
  if (decpB > 10)
    decpB /= 10;
    */
/*
  if (decpB > 5)
    decpA++;
  if (decpA >= 10)
  {
    decpA /= 10;
    ones++;
  }
  */
  //Serial.println("---");
  myvin = getVolt(A2);
  //Serial.println("---");
  //pvin = readVcc(); // /= 1000.0; // convert millivolt to volt
  //Serial.print("i ");  Serial.print(pvin);
  //Serial.print(" v ");
  //Serial.print(tens);  Serial.print(ones);  Serial.print(".");  Serial.print(decpA);  Serial.println(decpB);
  //Serial.println(myvin);
  digitalWrite(LED_BUILTIN, (myvin < 4 && myvin != 0));
  //Serial.println("---");
  delay(1000);
}

float getVolt(int aPin)
{
  float retVal = 0.0;
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES)
  {
    sum += analogRead(aPin);
    sample_count++;
    delay(10);
  }
  // calculate the voltage
  // use 5.0 for a 5.0V ADC reference voltage
  // 5.015V is the calibrated reference voltage
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.09) / 1024.0;
  //voltage = ((float)sum / (float)NUM_SAMPLES * readVcc()) / 1024.0;
  // send voltage for display on Serial Monitor
  // voltage multiplied by 11 when using voltage divider that
  // divides by 11. 11.132 is the calibrated voltage divide
  // value
  retVal = voltage * 11.069;
  if (retVal < 0.09)
    retVal = 0.0;
  sample_count = 0;
  sum = 0;
  //Serial.print("v "); Serial.println(retVal);
  return retVal;
}

float readVcc()
{
  float t;
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
  t = (float)result / 1000; // convert millivolts to volts
 // Serial.print("t "); Serial.println(t);
  return t; // Vcc in volts
}
