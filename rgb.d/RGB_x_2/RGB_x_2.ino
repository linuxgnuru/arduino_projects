//#define PWR 1

const int rPinA = 11;
const int gPinA = 10;
const int bPinA = 9;

const int rPinB = 6;
const int gPinB = 5;
const int bPinB = 3;

long vcc;

void setup()
{
  pinMode(rPinA, OUTPUT);
  pinMode(gPinA, OUTPUT);
  pinMode(bPinA, OUTPUT);
  pinMode(rPinB, OUTPUT);
  pinMode(gPinB, OUTPUT);
  pinMode(bPinB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  unsigned int rgbA[3];
  unsigned int rgbB[3];
  int rev_dec;

#ifdef PWR
  Serial.print("vcc: ");
  Serial.println(readVcc());
#endif
  rgbA[0] = rgbB[2] = 255;
  rgbA[1] = rgbA[2] = rgbB[0] = rgbB[1] = 0;
  for (int decrement = 0; decrement < 3; decrement++)
  {
    rev_dec = abs(decrement - 2);
#ifdef DEBUG
    Serial.print("dec: ");
    Serial.print(decrement);
    Serial.print(" rev_dec: ");
    Serial.println(rev_dec);
#endif
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
#ifdef DEMO
  color(255, 0, 0);
  delay(1000);
  color(0, 255, 0);
  delay(1000);
  color(0, 0, 255);
  delay(1000);
  color(237, 109, 0);
  delay(1000);
  color(255, 215, 0);
  delay(1000);
  color(34, 139, 34);
  delay(1000);
  color(0, 46, 90);
  delay(1000);
  color(128, 0, 128);
  delay(1000);
#endif
}

void color(int rPin, int bPin, int gPin, unsigned char red, unsigned char green, unsigned char blue)
{
  analogWrite(rPin, red);
  analogWrite(bPin, blue);
  analogWrite(gPin, green);
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

