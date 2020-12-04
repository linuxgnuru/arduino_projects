#define DEBUG

#include <DebugUtils.h>

#include <NS_energyShield.h>

#include "motors.h"

NS_energyShield eS; // Create NS-energyShield object called "eS"
int Voltage, Current, AdapterVoltage;
float Temperature;
unsigned int lastMillisIntBat = 0;

const byte voltPin = A2;
boolean extBatAvail = true;
unsigned int lastMillisCheckBatt = 0;
unsigned int lastMillisBat = 0;

int switchPins[] = {
  6, 7
};

const byte mPins[2][3] = {
  // EN  IN_A IN_B
  { 11, 2, 3 }, // motor 0 right
  { 10, 4, 5 }  // motor 1 left
};

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 2; i++)
  {
    pinMode(switchPins[i], INPUT_PULLUP);
    for (int j = 0; j < 3; j++)
      pinMode(mPins[i][j], OUTPUT);
  }
}

void loop()
{
  unsigned int currentMillis = millis();
  Voltage = eS.voltage(); // Voltage is returned in mV
  Current = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature() / 8; // Temperature is returned in 0.125 C increments
  AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage
  if (currentMillis - lastMillisIntBat >= 1000)
  {
    lastMillisIntBat = currentMillis;
    DEBUG_PRINT(Voltage / 1000.0);
    DEBUG_PRINT(" V\t\t");
    DEBUG_PRINT(abs(Current));
    DEBUG_PRINT(" mA\t\t");
    DEBUG_PRINT(Percent);
    DEBUG_PRINT(" %\t\t");
    DEBUG_PRINT(Temperature);
    DEBUG_PRINT(" C\t\t");
    DEBUG_PRINT(AdapterVoltage / 1000.0);
    DEBUG_PRINTLN(" V");
  }
  boolean rev_r = digitalRead(switchPins[S_R]);
  boolean rev_l = digitalRead(switchPins[S_L]);
  if (rev_r && rev_l)
  {
    setMotor(M_L, rev_l);
    setMotor(M_R, rev_r);
  }
  else if (rev_r && !rev_l)
  {
    setMotor(M_L, rev_l);
    stopMotor(M_R);
  }
  else if (rev_l && !rev_r)
  {
    setMotor(M_R, rev_r);
    stopMotor(M_L);
  }
  else if (!rev_l && !rev_r)
  {
    stopMotor(M_L);
    stopMotor(M_R);
  }
  if (currentMillis - lastMillisBat >= 3000)
  {
    lastMillisBat = currentMillis;
    checkBatt();
    if (!extBatAvail)
      Serial.println(F("No external battery"));
  }
}

void setMotor(int motor, boolean reverse)
{
  digitalWrite(mPins[motor][EN], HIGH);
  digitalWrite(mPins[motor][IN_A], !reverse);
  digitalWrite(mPins[motor][IN_B], reverse);
}

void stopMotor(int motor)
{
  digitalWrite(mPins[motor][EN], HIGH);
  digitalWrite(mPins[motor][IN_A], LOW);
  digitalWrite(mPins[motor][IN_B], LOW);
}

void checkBatt()
{
  unsigned int currentMillis = millis();
  if (currentMillis > lastCheckBattTime + 100)
    isCharging();
  if (currentMillis % 5000 == 0)
  {
    // don't test if we are charging
    if (!charging)
    {
      // internal battery
      Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
      if (Percent < 30)
        Serial.println(F("low internal battery"));
    }
  }
  // external battery
  if (extBatAvail)
  {
    if (getVolt() < 4)
      Serial.println(F("External battery is low"));
  }
  else // no battery was found
  {
    // every 10 seconds, test to see if the external battery is now present or not; resets extBatAvail flag
    if (currentMillis - lastMillisCheckBatt >= 10000)
    {
      lastMillisCheckBatt = currentMillis;
      getVolt();
    }
  }
}

float getVolt()
{
  float vin = 0.0;
  float vout = 0.0;
  float R1 = 998000.0;
  float R2 = 100000.0;
  int value;

  value = analogRead(voltPin);
  vout = (value * readVcc()) / 1024.0;
  //DEBUG_PRINTFLN("rVcc", vout);
  //vout = (value * 5.04) / 1024.0;
  //DEBUG_PRINTFLN("5.04", vout);
  vin = vout / (R2 / (R1 + R2));
  //DEBUG_PRINTFLN("vin", vin);
  if (vin < 0.09)
    vin = 0.0;
  // test for external battery
  extBatAvail = (vin >= 5.4);
  Serial.print(F("ext bat: "));
  Serial.println(vin);
  return vin;
}

// get the actual voltage the arduino has
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
  // -=-=-=-=-=-=-= DELAY =-=-=-=-=-=-
  delay(2); // Wait for Vref to settle
  // -=-=-=-=-=-=-= DELAY =-=-=-=-=-=-
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high << 8) | low;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  t = (float)result / 1000;
  return t; // Vcc in millivolts
}

void isCharging()
{
  for (int i = 9; i > 0; i--)
    chCurrent[i] = chCurrent[i - 1];
  chCurrent[0] = eS.current();
  for (int i = 9; i >= 0; i--)
  {
    if (chCurrent[i] < 0)
    {
      chCurrent[i] = -chCurrent[i];
      charging = false;
    }
    else
      charging = true;
  }
  lastCheckBattTime = millis();
  //if (charging)    DEBUG_PRINTLN("- charging -");
}

