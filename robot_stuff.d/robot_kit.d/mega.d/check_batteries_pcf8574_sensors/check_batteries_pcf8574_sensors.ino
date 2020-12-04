#include <NS_energyShield.h>
#include <PCF8574.h>
#include <Wire.h>

#define DEBUG
unsigned long counter = 0;

// The following was a test
#define ALL_OK 0
#define DOWN_F 1
#define DOWN_B 2
#define BACK_B 4

/* BATTERY */

// FIXME should we do samples? uses delay...
#define NUM_SAMPLES 10

#define NEW_WAY
// #define OLD_WAY

NS_energyShield eS; // Create NS-energyShield object called "eS"

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
float myVin = 0.0;

float Percent; // percentage of internal battery remaining

long lastTime = millis();

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

void setup()
{
  pcf20.begin();
  //attachInterrupt(0, catchChange, FALLING);
  attachInterrupt(0, catchChange, CHANGE);
  pcf20.write(runningOKLed, LOW);
#ifdef DEBUG
  Serial.begin(9600);
#endif
  checkBatt();
}

void loop()
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillisS >= 50)
  {
    lastMillisS = currentMillis;
    if (hasChanged)
    {
      hasChanged_fd = pcf20.readButton(frontDownSensorPin);
      hasChanged_bd = pcf20.readButton(backDownSensorPin);
      hasChanged_bu = !pcf20.readButton(backUpSensorPin);
      if (hasChanged_fd || hasChanged_bd || hasChanged_bu)
      {
        if ((hasChanged_fd && hasChanged_bd) || (hasChanged_fd && hasChanged_bd && hasChanged_bu))
        {
          Serial.println(F("-=-=-= ERROR =-=-=-"));
          blinkLeds();
          /*
            boolean go = true;
            while (go)
            {
            blinkLeds();
            hasChanged_fd = pcf20.readButton(frontDownSensorPin);
            hasChanged_bd = pcf20.readButton(backDownSensorPin);
            hasChanged_bu = !pcf20.readButton(backUpSensorPin);
            if ((hasChanged_fd && hasChanged_bd) || (hasChanged_fd && hasChanged_bd && hasChanged_bu))
              go = false;
            }
          */
        }
        else
        {
          pcf20.write(backingUpLed, LOW);
          pcf20.write(runningOKLed, HIGH);
          Serial.println(F("===="));
          if (hasChanged_fd)
          {
            // add code to do stuff
            //Serial.println(F("front down"));
            Serial.print(F("fd "));
            Serial.println(hasChanged_fd);
          }
          if (hasChanged_bd)
          {
            // add code to do stuff
            //Serial.println(F("back down"));
            Serial.print(F(" bd "));
            Serial.println(hasChanged_bd);
          }
          if (hasChanged_bu)
          {
            // add code to do stuff
            //Serial.println(F("back away"));
            Serial.print(F(" bu "));
            Serial.println(hasChanged_bu);
          }
          Serial.println(F("-----"));
        }
      }
      else
      {
        pcf20.write(runningOKLed, LOW);
        pcf20.write(backingUpLed, HIGH);
      }
      hasChanged = false;
    }
  }
  // only check the battery every 3 minutes
  // and then, only if we have to stop or something.
  //  else if (currentMillis - lastMillis >= 180000 && (pcf20.read(backingUpLed)))
  if (!extBatAvail)
  {
    //pcf20.write(runningOKLed, LOW);
    blink_extBat();
  }
#ifdef DEBUG
  else
  {
    unsigned int currentMillis_tmp = millis();
    if (currentMillis_tmp - lastMillisDEBUG >= 5000)
    {
      lastMillisDEBUG = currentMillis_tmp;
      //Serial.print(F("extBatAvail: "));      Serial.print(extBatAvail);
      Serial.print(F(" myVin: "));      Serial.println(myVin);
    }
  }
#endif
  //if (currentMillis - lastMillisB >= 180000 && (pcf20.read(backingUpLed)))
  if (currentMillis - lastMillisB >= 3000 && (pcf20.read(backingUpLed)))
  {
    //Serial.println(F("checking batteries"));
    lastMillisB = currentMillis;
    checkBatt();
  }
  //delay(500);
}

/* BEGIN BATTERY */
void checkBatt()
{
  unsigned int currentMillis = millis();
  // internal battery
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  pcf20.write(lowBatIntLed, !(Percent < 30));

  // external battery
  if (extBatAvail)
  {
    myVin = getVolt(voltPin);
    if (myVin > 3)
    {
      pcf20.write(lowBatExtLed, !(myVin < 4));//!((millis() % 3000 == 0) && myVin < 4));
      pcf20.write(runningOKLed, ((Percent < 30) || (myVin < 4)));
    }
    else
    {
      extBatAvail = false;
    }
  }
  else
  {
    pcf20.write(runningOKLed, (Percent < 30));
    if (currentMillis - lastMillisCB >= 10000)
    {
      lastMillisCB = currentMillis;
      myVin = getVolt(voltPin);
      if (myVin > 3)
        extBatAvail = true;
    }
  }
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
#ifdef DEBUG
  Serial.println(F("-*-*-*-*"));
  Serial.print(F("sum: "));
  Serial.print(sum);
  Serial.print(F(" voltage: "));
  Serial.print(voltage);
  Serial.print(F(" vcc: "));
  Serial.print(readVcc());
  Serial.print(F(" vin: "));
  Serial.print(vin);
  Serial.println(F("-#-#-#-"));
  //Serial.print(F(" "));  Serial.print();
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
#ifdef DEBUG
  Serial.print(F("vout: "));
  Serial.print(vout);
  Serial.print(F(" vin: "));
  Serial.println(vin);
#endif
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
  if (currentMillis - lastMillisBlinkE >= 50)
  {
    lastMillisBlinkE = currentMillis;
    pcf20.write(backingUpLed, !pcf20.read(backingUpLed));
    //    pcf20.write(runningOKLed, !pcf20.read(runningOKLed));
  }
}

boolean stop_me = false;

void blink_extBat()
{
  unsigned int currentMillis = millis();
  // only blink for 3 minutes
  if (!stop_me && currentMillis - lastMillisBlinkKill >= 180000)
  {
    lastMillisBlinkKill = currentMillis;
    stop_me = true;
  }
  if (!stop_me && currentMillis - lastMillisBlink >= interval)
  {
    lastMillisBlink = currentMillis;
    pcf20.write(lowBatExtLed, !pcf20.read(lowBatExtLed));
  }
}


