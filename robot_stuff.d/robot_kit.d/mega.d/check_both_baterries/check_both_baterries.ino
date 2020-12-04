#include <NS_energyShield.h>
#include <Wire.h>

#define BATT_CAP 1200 // mAh
#define NUM_SAMPLES 10

#define DEBUG 1

NS_energyShield eS; // Create NS-energyShield object called "eS"

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
float myVin = 0.0;

unsigned long TimeToEmpty;
int Current[10], AvgCurr;
boolean charging;

long lastTimeA = millis();
long lastTimeB = millis();

const byte voltPin = A1;
const byte lowBatInt = LED_BUILTIN;
const byte lowBatExt = 12;

boolean first_time = true;

typedef enum {
  INTERNAL_BATT,
  EXTERNAL_BATT
}
battery;

void setup()
{
  pinMode(lowBatInt, OUTPUT);
  pinMode(lowBatExt, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  checkBatt(INTERNAL_BATT);
  checkBatt(EXTERNAL_BATT);
}

void checkBatt(int type)
{
  switch (type)
  {
    case INTERNAL_BATT:
      if (millis() > lastTimeA + 100)
        TTE();
      if (millis() % 5000 == 0)
      {
        if (!charging && !first_time)
        {
          first_time = false;
          digitalWrite(lowBatInt, (((TimeToEmpty / 60) <= 0) && ((TimeToEmpty % 60) <= 5)));
//#ifdef DEBUG
#if 0
          Serial.print(F("int: "));
          Serial.print(TimeToEmpty / 60);
          Serial.print(F(":"));
          Serial.println(TimeToEmpty % 60);
#endif
        }
        delay(1); // Ensure that a ms passes, so it does not double print
      }
      break;
    case EXTERNAL_BATT:
      if (millis() > lastTimeB + 100)
        myVin = getVolt(voltPin);
      if (millis() % 3000 == 0)
      {
        digitalWrite(lowBatExt, (myVin < 4));
#ifdef DEBUG
        Serial.print(F("ext: "));
        Serial.println(myVin);
#endif
      }
      delay(1);
      break;
  }
}

void TTE()
{
  for (int i = 9; i > 0; i--)
    Current[i] = Current [i - 1];
  Current[0] = eS.current();
  for (int i = 9; i >= 0; i--)
  {
    if (Current[i] < 0)
    {
      Current[i] = -Current[i];
      charging = 0;
    }
    else
      charging = 1;
  }
  // Check that energyShield is not charging
  for (int i = 0; i < 10; i++)
    AvgCurr += Current[i];
  AvgCurr /= 10;
  // Minutes until empty
  TimeToEmpty = (unsigned long) BATT_CAP * eS.percent() * 60 / AvgCurr / 200;
  lastTimeA = millis();
}

float getVolt(int aPin)
{
  float vin = 0.0;
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
  #if 0 //def DEBUG
  Serial.print(F("sum: "));
  Serial.print(sum);
  Serial.print(F(" voltage: "));
  Serial.print(voltage);
  Serial.print(F(" vcc: "));
  Serial.print(vcc);
  Serial.print(F(" vin: "));
  Serial.println(vin);
  //Serial.print(F(" "));  Serial.print();
  #endif
  sample_count = 0;
  sum = 0;
  // not sure why it's off by 2
  vin -= 2;

  #if 0
  // old way
  float vout = 0.0;
  float R1 = 100000.0;
  float R2 = 10000.0;
  int value;

  value = analogRead(aPin);
  vout = (value * readVcc()) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
#endif
  if (vin < 0.09)
    vin = 0.0;
  lastTimeB = millis();
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

