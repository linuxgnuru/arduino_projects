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

#include <SPI.h>

#include "segment_digit.h"

#define NUM_SAMPLES 10

//#define DEBUG 1

const byte LATCH = 10;

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage

// for shift register leds:
byte data[] = { 
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

void setup()
{
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pinMode(LATCH, OUTPUT);
  SPI.begin();
  // following line sets the RTC to the date & time this sketch was compiled
//  RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{
  int tens, ones, tmp, decpA, decpB;
  float myvin = getVolt(A3);
  tens = (int)myvin / 10;
  ones = (int)myvin % 10;
  tmp = myvin * 10;
  decpA = (int)tmp % 10;
  decpB = (int)tmp % 100;
  if (decpB > 10)
    decpB /= 10;
#if 0
  Serial.println(myvin);
  Serial.print(tens);
  Serial.print(ones);
  Serial.print(".");
  Serial.print(decpA);
  Serial.println(decpB);
#endif

  printDigit(0, tens, false);
  printDigit(1, ones, true);
  printDigit(2, decpA, false);
  printDigit(3, decpB, false);
  delay(1000);
}

// pos = 0 - 3
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
    digitalWrite(LATCH, LOW);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    SPI.transfer(data[ctr++]);
    digitalWrite(LATCH, HIGH);
  }
}

void clearD(int pos)
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  digitalWrite(LATCH, HIGH);
}

#if 0
float getVolt(int aPin)
{
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 998000.0;
  float R2 = 100000.0;
  int value;

  //float myVcc = readVcc();  Serial.println(myVcc);
  value = analogRead(aPin);
  vout = (value * 4.99) / 1023.0;
  //vin = vout / (R2 / (R1 + R2));
  vin = 
  if (vin < 0.09)
    vin = 0.0;
  return vin;
}


#endif

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
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.03) / 1024.0;
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
  t = (float)result / 1000;
  return t; // Vcc in millivolts
}
