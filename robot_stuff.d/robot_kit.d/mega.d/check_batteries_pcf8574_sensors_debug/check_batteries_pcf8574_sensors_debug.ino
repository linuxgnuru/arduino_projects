#include <NS_energyShield.h>
#include <PCF8574.h>
#include <Wire.h>

#define DEBUG
//#define INT_BAT_ONLY
//#define EXT_BAT_DEBUG

#include <DebugUtils.h>

// The following was a test
#define ALL_OK 0
#define DOWN_F 1
#define DOWN_B 2
#define BACK_B 4

#define NO_EXT_CHECK

/* BATTERY */

#define NEW_WAY
// #define OLD_WAY

#ifdef NEW_WAY
// FIXME should we do samples? uses delay...
#define NUM_SAMPLES 10
#endif

boolean sens_hit_flag = false;
boolean stop_me = false;

NS_energyShield eS; // Create NS-energyShield object called "eS"

// for internal battery charging test

long lastCheckTime = millis();
int chCurrent[10];
boolean charging;

#ifdef NEW_WAY
int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
#endif

float myVin = 0.0;

#ifdef INT_BAT_ONLY
int Voltage, Current, AdapterVoltage;
float Percent, Temperature;
#else
float Percent; // percentage of internal battery remaining
#endif

const byte voltPin = A2;

/* END BATTERY */

/* BEGIN PCF8574 and sensor */

PCF8574 pcf20(0x20);

const byte frontDownSensorPin = 0;
const byte backDownSensorPin  = 1;
const byte backUpSensorPin  = 2;

const byte runningOKLed = 3;
const byte lowBatIntLed = 4;
const byte lowBatExtLed = 5;
const byte backingUpLed = 6;


volatile boolean hasChanged = false;

boolean hasChanged_fd = false;
boolean hasChanged_bd = false;
boolean hasChanged_bu = false;

/* END SENSOR */

boolean extBatAvail = true;

const long interval = 1000;

unsigned int lastMillisS = 0;
unsigned int lastMillisB = 0;
unsigned int lastMillisBA = 0;
unsigned int lastMillisBlink = 0;
unsigned int lastMillisBlinkE = 0;
unsigned int lastMillisCB = 0;
unsigned int lastMillisDEBUG = 0;
unsigned int lastMillisBlinkKill = 0;

#ifdef INT_BAT_ONLY
unsigned int lastMillisV = 0;
#endif


void setup()
{
  pcf20.begin();
  //attachInterrupt(0, catchChange, FALLING);
  attachInterrupt(0, catchChange, CHANGE);
  pcf20.write(runningOKLed, LOW);
#ifdef DEBUG
  Serial.begin(9600);
#ifdef INT_BAT_ONLY
  while (!Serial); // Wait for open COM port
  delay(5000);
//  DEBUG_PRINTLN("Voltage (V)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (V)\tIs Charging?");
  DEBUG_PRINTLN("Voltage (V)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (V)");
#endif
#endif
//  DEBUG_PRINTLN("- Debug enabled -");
#ifndef NO_EXT_CHECK
  checkBatt();
#endif
}

void loop()
{
#ifdef INT_BAT_ONLY
  Voltage = eS.voltage(); // Voltage is returned in mV
  Current = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature() / 8; // Temperature is returned in 0.125 C increments
  AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage
  unsigned int currentMillisV = millis();
  if (currentMillisV - lastMillisV >= 1000)
  {
    lastMillisV = currentMillisV;
    isCharging();
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
/*
    DEBUG_PRINT(" V\t\t");
    DEBUG_PRINTLN(charging);
    */
  }
#endif
  unsigned int currentMillis = millis();
  // since we are using interrupts, we might not need this test...
  /*
    // Limit button read to 20 times a second.  Fast enough for most buttons but
    // this way you don't have a dimmer output because it's blanked during button
    // read a read takes 460us t 16Mhz Arduino and normal I2C speed.
    if (currentMillis - lastMillisS >= 50)
    {
      lastMillisS = currentMillis;
  */
  if (hasChanged)
  {
    // the front and back will show 0/LOW/false if everything is ok
    hasChanged_fd = pcf20.readButton(frontDownSensorPin);
    hasChanged_bd = pcf20.readButton(backDownSensorPin);
    // the behind sensor gives 1/HIGH/true if everything is ok
    hasChanged_bu = !pcf20.readButton(backUpSensorPin);
    if (hasChanged_fd || hasChanged_bd || hasChanged_bu)
    {
      // both front and back sensors say there's nothing under them or every sensor is triggered
      if ((hasChanged_fd && hasChanged_bd) || (hasChanged_fd && hasChanged_bd && hasChanged_bu))
      {
        DEBUG_PRINTLN("-=-=-= ERROR =-=-=-");
        blinkLeds();
      }
      else
      {
        pcf20.write(backingUpLed, LOW);
        pcf20.write(runningOKLed, HIGH);
        DEBUG_PRINTLN("====");
        if (hasChanged_fd)
        {
          DEBUG_PRINTFLN("fd", hasChanged_fd);
        }
        if (hasChanged_bd)
        {
          DEBUG_PRINTFLN("bd", hasChanged_bd);
        }
        if (hasChanged_bu)
        {
          DEBUG_PRINTFLN("bu", hasChanged_bu);
        }
        DEBUG_PRINTLN("-----");
        delay(1);
      }
      sens_hit_flag = false;
    }
    else // no sensors have been flagged
    {
      if (!sens_hit_flag)
      {
        sens_hit_flag = true;
        DEBUG_PRINTLN("no sensors hit");
      }
      if (!stop_me)
        pcf20.write(runningOKLed, LOW);
      pcf20.write(backingUpLed, HIGH);
    }
    hasChanged = false;
  } // end hasChanged
  /*
    } // end millis test
  */
  // blink the low external battery led if no battery is detected
#ifndef NO_EXT_CHECK
  if (!extBatAvail)
  {
    blink_extBat();
  }
  // only check the battery every 3 minutes and then, only if we have to stop or something.
  //if (currentMillis - lastMillisB >= 180000 && (pcf20.read(backingUpLed)))
  // check battery status every 3 seconds
  if (currentMillis - lastMillisB >= 3000)
  {
    //DEBUG_PRINTLN("checking batteries");
    lastMillisB = currentMillis;
    checkBatt();
  }
  //delay(500);
#endif
}

/* BEGIN BATTERY */
void checkBatt()
{
  if (millis() > lastCheckTime + 100)
    isCharging();
  if (millis() % 5000 == 0)
  {
    // don't test if we are charging
    if (!charging)
    {
      // internal battery
      Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
      pcf20.write(lowBatIntLed, !(Percent < 30));
    }
    else
    {
      pcf20.write(lowBatIntLed, HIGH);
      if (!stop_me)
        pcf20.write(runningOKLed, LOW);
    }
  }
  // external battery
  if (extBatAvail)
  {
    myVin = getVolt(voltPin);
    if (myVin > 3)
    {
      pcf20.write(lowBatExtLed, !(myVin < 4));//!((millis() % 3000 == 0) && myVin < 4));
      if (!stop_me)
        pcf20.write(runningOKLed, (((!charging) && Percent < 30) || (myVin < 3)));
    }
    else
    {
      extBatAvail = false;
    }
  }
  else // no battery was found
  {
    // turn off the running ok led if the percent is under 30
    if (!stop_me)
      pcf20.write(runningOKLed, ((!charging) && Percent < 30));
    // every 10 seconds, test to see if the external battery is now present or not; resets extBatAvail flag
    unsigned int currentMillis = millis();
    if (currentMillis - lastMillisCB >= 10000)
    {
      lastMillisCB = currentMillis;
      myVin = getVolt(voltPin);
      if (myVin > 3)
        extBatAvail = true;
    }
  }
  //  delay(1);
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
#ifdef EXT_BAT_DEBUG
  DEBUG_PRINTLN("-*-*-*-*");
  //DEBUG_PRINTF("sum", sum);
  DEBUG_PRINTF("voltage", voltage);
  DEBUG_PRINTF(" readVcc", readVcc());
  DEBUG_PRINTFLN(" vin", vin);
  DEBUG_PRINTLN("-#-#-#-");
  //DEBUG_PRINT(" ");  DEBUG_PRINT();
#endif
  sample_count = 0;
  sum = 0;
  // not sure why it's off by 2
  vin -= 2;
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
#ifdef EXT_BAT_DEBUG
  DEBUG_PRINTF("value", value);
  DEBUG_PRINTF(" R1", R1);
  DEBUG_PRINTF(" R2", R2);
  DEBUG_PRINTF(" vout", vout);
  DEBUG_PRINTFLN(" vin", vin);
#endif
#endif
  if (vin < 0.09)
    vin = 0.0;
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

/* END BATTERY */

/* BEGIN PCF8574 INTERRUPT FUNCTION */

void catchChange()
{
  hasChanged = true;
}

/* END PCF8574 */

void blinkLeds()
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillisBlinkE >= 100)
  {
    DEBUG_PRINTLN("blink blue");
    lastMillisBlinkE = currentMillis;
    pcf20.write(backingUpLed, !pcf20.read(backingUpLed));
    //    pcf20.write(runningOKLed, !pcf20.read(runningOKLed));
  }
}

void blink_extBat()
{
  unsigned int currentMillisSTOP = millis();
  unsigned int currentMillisGO = millis();
  // only blink for 3 minutes
  //if (!stop_me && currentMillisSTOP - lastMillisBlinkKill >= 180000)
  if (!stop_me && currentMillisSTOP - lastMillisBlinkKill >= 30000)
  {
#ifndef INT_B_ONLY
    DEBUG_PRINTLN("3 minutes has past!!");
#endif
    lastMillisBlinkKill = currentMillisSTOP;
    stop_me = true;
  }
  if (!stop_me && currentMillisGO - lastMillisBlink >= interval)
  {
    lastMillisBlink = currentMillisGO;
    pcf20.write(lowBatExtLed, !pcf20.read(lowBatExtLed));
  }
  if (stop_me)
  {
    pcf20.write(runningOKLed, HIGH);
    pcf20.write(lowBatExtLed, HIGH);
    pcf20.write(lowBatIntLed, HIGH);
  }
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
      charging = 0;
    }
    else
      charging = 1;
  }
  lastCheckTime = millis();
  //if (charging)    DEBUG_PRINTLN("- charging -");
}

