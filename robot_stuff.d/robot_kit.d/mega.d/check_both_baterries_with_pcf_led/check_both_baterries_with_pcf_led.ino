#include <NS_energyShield.h>
#include <Wire.h>
#include <PCF8574.h>

// FIXME should we do samples? uses delay...
#define NUM_SAMPLES 10
NS_energyShield eS; // Create NS-energyShield object called "eS"

PCF8574 pcf20(0x20);

#define DEBUG
#define NEW_WAY
// #define OLD_WAY

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
float myVin = 0.0;

float Percent;

long lastTime = millis();

const byte voltPin = A2;

const byte runningOKLed = 3;
const byte lowBatIntLed = 4;
const byte lowBatExtLed = 5;
const byte backingUpLed = 6;

void setup()
{
  pcf20.begin();
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pcf20.write(runningOKLed, LOW);
}

void loop()
{
  checkBatt();
#ifdef DEBUG
  Serial.print(F("percent: "));
  Serial.print(Percent);
#endif
  delay(1000);
}

void checkBatt()
{
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  pcf20.write(lowBatIntLed, !(Percent < 30));
  myVin = getVolt(voltPin);
  pcf20.write(lowBatExtLed, !(myVin < 4));//!((millis() % 3000 == 0) && myVin < 4));
  pcf20.write(runningOKLed, ((Percent < 30) || (myVin < 4)));
  delay(1);
}

float getVolt(int aPin)
{
  float vin = 0.0;
#ifdef NEW_WAY
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES)
  {
    sum += analogRead(aPin);
    sample_count++;
    delay(10);
  }
  // calculate the voltage
  // use 5.0 for a 5.0V ADC reference voltage
  // 5.015V is the calibrated reference voltage 5.03
  //voltage = ((float)sum / (float)NUM_SAMPLES * vcc) / 1024.0;
  voltage = ((float)sum / (float)NUM_SAMPLES * readVcc()) / 1024.0;
  // send voltage for display on Serial Monitor
  // voltage multiplied by 11 when using voltage divider that
  // divides by 11. 11.132 is the calibrated voltage divide
  // value
  vin = voltage * 11.069;
#def DEBUG
  Serial.print(F("sum: "));
  Serial.print(sum);
  Serial.print(F(" voltage: "));
  Serial.print(voltage);
  Serial.print(F(" vcc: "));
  Serial.print(readVcc());
  Serial.print(F(" vin: "));
  Serial.println(vin);
  //Serial.print(F(" "));  Serial.print();
#endif
  sample_count = 0;
  sum = 0;
  // not sure why it's off by 2
  vin -= 2;
#endif
#else
  // old way
  float vout = 0.0;
  float R1 = 100000.0;
  float R2 = 10000.0;
  int value;

  value = analogRead(aPin);
  //  vout = (value * readVcc()) / 1024.0;
  vout = (value * 5.04) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
#ifdef DEBUG
  Serial.print(F("vout: "));
  Serial.print(vout);
  Serial.print(F(" vin: "));
  Serial.println(vin);
#endif
  if (vin < 0.09)
    vin = 0.0;
  lastTime = millis();
  return vin;
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
  t = (float)result / 1000;
  return t; // Vcc in millivolts
}

