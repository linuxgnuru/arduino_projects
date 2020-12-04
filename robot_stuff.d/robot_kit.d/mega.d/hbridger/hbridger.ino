/*
   Pinout for L293D H-driver
          +----   ----+
   1,2EN [| 1  |_| 16 |] Vcc1
      1A [| 2      15 |] 4A
      1Y [| 3      14 |] 4Y
     GND [| 4      13 |] GND
     GND [| 5      12 |] GND
      2Y [| 6      11 |] 3Y
      2A [| 7      10 |] 3A
    Vcc2 [| 8       9 |] 3,4EN
          +-----------+
   pinout for ISCCSCSP thing

   [12] [5v]
   [13] [11]
   [rs] [GD]
*/
#include <NS_energyShield.h>
#include <PCF8574.h>
#include <Wire.h>
#include <Sweeper.h>
#include <NewPing.h>

#define DEBUG

#include <DebugUtils.h>

#include "vars.h"

#define SERVO_ENABLE
#define SENSOR_ENABLE
#define SONAR_ENABLE
//#define NO_EXT_BAT_CHECK
//#define NO_MOTOR
#define INT_BAT_DEBUG

#define BACKUP_DELAY 2000


#ifdef SERVO_ENABLE

#define SERVO_NUM 2
Sweeper sweeper[SERVO_NUM] {
  Sweeper(8, 9),
  Sweeper(8, 10)
};

#endif

PCF8574 pcf20(0x20);

NS_energyShield eS; // Create NS-energyShield object called "eS"

#ifdef INT_BAT_DEBUG
int Voltage, Current, AdapterVoltage;
float Temperature;
unsigned int lastMillisIntBat = 0;
#endif

/*   User changable variables here:  */

#ifdef SONAR_ENABLE
// Sensor object array.
// Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing sonar[SONAR_NUM] = {
  //NewPing(TRIG, ECHO, MAX_DISTANCE)
  NewPing(A3, 12, MAX_DISTANCE), // (facing front) left
  NewPing(A1, A0, MAX_DISTANCE), // center
  NewPing( 4, 11, MAX_DISTANCE)  // right
};
#endif

// to use this method:  mPins[MOTOR_R][EN]
const byte mPins[2][3] = {
  // EN  IN_A IN_B
  { 5, 8, 2 }, // motor 0 right
  { 6, 3, 7 }  // motor 1 left
};

const byte voltPin = A2;

//const byte PCF_motor_IN1 = 0; // was pin 8 (IN_A, IN1)
const byte PCF_noBatPin = 0;
const byte PCF_backUpSensorPin  = 1;
const byte PCF_backDownSensorPin  = 2;
const byte PCF_frontDownSensorPin = 3;
const byte PCF_okLed = 4;
const byte PCF_lowBatIntLed = 5;
const byte PCF_lowBatExtLed = 6;
const byte PCF_backingUpLed = 7;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef SERVO_ENABLE
  //sweeper0.Attach(sweeper0_pin);
  for (int i = 0; i < SERVO_NUM; i++)
    sweeper[i].Attach();
#endif
  pcf20.begin();
  // h-bridge
  for (int i = 0; i < 3; i++)
  {
    pinMode(mPins[MOTOR_R][i], OUTPUT);
    pinMode(mPins[MOTOR_L][i], OUTPUT);
  }
#ifdef SONAR_ENABLE
  // SONAR
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
#endif
  //attachInterrupt(0, catchChange, FALLING);
  //attachInterrupt(0, catchChange, CHANGE);
  //pcf20.write(PCF_okLed, LOW);
#ifdef DEBUG
  Serial.begin(9600);
#ifdef INT_BAT_DEBUG
  unsigned int currentMillis, waitMillis, waitBMillis;
  boolean flag = true;
  waitMillis = waitBMillis = 0;
  // Wait for open COM port
  while (!Serial)
    ;
  pcf20.write(7, LOW);
  do
  {
    currentMillis = millis();
    if (currentMillis - waitBMillis >= 500)
    {
      waitBMillis = currentMillis;
      pcf20.write(5, !pcf20.read(5));
      pcf20.write(7, !pcf20.read(7));
    }
    if (currentMillis - waitMillis >= 5000)
    {
      waitMillis = currentMillis;
      flag = false;
    }
  } while (flag);
  pcf20.write(5, HIGH);
  pcf20.write(7, HIGH);
  DEBUG_PRINTLN("Voltage (V)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (V)");
#endif
#endif
  checkBatt();
}

void loop()
{
  unsigned int currentMillis = millis();
#ifdef INT_BAT_DEBUG
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
    /*
      isCharging();
      DEBUG_PRINT(" V\t\t");
      DEBUG_PRINTLN(charging);
    */
  }
#endif
#ifdef SONAR_ENABLE
  Sonar();
#endif
#ifdef SERVO_ENABLE
  if (extBatAvail)
  {
    for (int i = 0; i < SERVO_NUM; i++)
      sweeper[i].Update();
    /*
      if (problemState == A_OK)
      {
      for (int i = 0; i < SERVO_NUM; i++)
        sweeper[i].Update();
      }
      else
      {
      //sweeper[0].Park(true);
      sweeper[1].Update();
      }
    */
  }
  else
  {
    for (int i = 0; i < SERVO_NUM; i++)
    {
      sweeper[i].Park(true);
    }
  }

#endif
  if ((emergStop || stopMe) && (currentMillis - lastMillisStopped >= 30000))
  {
    lastMillisStopped = currentMillis;
    stopMe = false;
    emergStop = false;
  }
#ifdef SENSOR_ENABLE
  // Limit button read to 20 times a second.  Fast enough for most buttons but
  // this way you don't have a dimmer output because it's blanked during button
  // read a read takes 460us t 16Mhz Arduino and normal I2C speed.
  if (currentMillis - lastMillisSensor >= 50)
  {
    lastMillisSensor = currentMillis;
    TestSensors();
  } // end millis test
#endif
  curSpeed = 180;
  if (!extBatAvail)
  {
    pcf20.write(PCF_okLed, HIGH);
    blink_(PCF_lowBatExtLed, 1000);
    /*
      for (int i = 0; i < SERVO_NUM; i++)
      {
      sweeper[i].Park(true);
      //sweeper[i].Detach();
      }
      }
      else
      {
      for (int i = 0; i < SERVO_NUM; i++)
      sweeper[i].Attach();
    */
  }
  else
  {
    pcf20.write(PCF_okLed, !(problemState == A_OK));
  }
  if (currentMillis - lastMillisBat >= 3000)
  {
    lastMillisBat = currentMillis;
    checkBatt();
  }
  if (!emergStop && !stopMe && extBatAvail)
  {
    // go forward
    if (problemState == A_OK)
    {
      curMotorNum = MOTOR_R;
      reverse[curMotorNum] = LOW;
      analogWrite( mPins[curMotorNum][EN], curSpeed);
      digitalWrite(mPins[curMotorNum][IN_A], !reverse[curMotorNum]); // IN1, true
      digitalWrite(mPins[curMotorNum][IN_B], reverse[curMotorNum]); // IN2, false
      curMotorNum = MOTOR_L;
      reverse[curMotorNum] = LOW;
      analogWrite( mPins[curMotorNum][EN], curSpeed);
      digitalWrite(mPins[curMotorNum][IN_A], !reverse[curMotorNum]);
      digitalWrite(mPins[curMotorNum][IN_B], reverse[curMotorNum]);
    }
    else
    {
      analogWrite( mPins[MOTOR_R][EN], 150);
      digitalWrite(mPins[MOTOR_R][IN_A], LOW);
      digitalWrite(mPins[MOTOR_R][IN_B], HIGH);
      analogWrite( mPins[MOTOR_L][EN], 150);
      digitalWrite(mPins[MOTOR_L][IN_A], LOW);
      digitalWrite(mPins[MOTOR_L][IN_B], LOW);
      if (currentMillis - lastMillisBackup >= BACKUP_DELAY)
      {
        lastMillisBackup = currentMillis;
        problemState = A_OK;
      }
    }
  }
#if 0
  else
  {
    // blink everything
    for (int i = 4; i < 8; i++)
    {
      pcf20.write(i, !pcf20.read(i));
      // ok to use delay here as everything is dead.
      delay(100);
    }
  }
#endif
}

/* BEGIN BATTERY */
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
      pcf20.write(PCF_lowBatIntLed, !(Percent < 30));
    }
    else
    {
      if (!pcf20.read(PCF_lowBatIntLed))
        pcf20.write(PCF_lowBatIntLed, HIGH);
    }
  }
  // external battery
  if (extBatAvail)
  {
    pcf20.write(PCF_lowBatExtLed, !(getVolt() < 4));//!((millis() % 3000 == 0) && getVolt() < 4));
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

/* END BATTERY */

void TestSensors()
{
  // the front and back will show 0/LOW/false if everything is ok
  hasChanged_fd = pcf20.readButton(PCF_frontDownSensorPin);
  // don't do this test if we are debuging
#ifdef DEBUG
  hasChanged_bd = hasChanged_bu = false;
  //hasChanged_fd = hasChanged_bd = hasChanged_bu = false;
#else
  hasChanged_bd = pcf20.readButton(PCF_backDownSensorPin);
  // the behind sensor gives 1/HIGH/true if everything is ok
  hasChanged_bu = !pcf20.readButton(PCF_backUpSensorPin);
#endif
  if (hasChanged_fd || hasChanged_bd || hasChanged_bu)
  {
    // TODO / FIXME the following isn't working for some reason...
    // both front and back sensors say there's nothing under them or every sensor is triggered
    if ((hasChanged_fd && hasChanged_bd) || (hasChanged_fd && hasChanged_bd && hasChanged_bu))
    {
      DEBUG_PRINTLN("-=-=-= ERROR =-=-=-");
      stopMe = true;
      //blinkLeds();
    }
    else
    {
      /*
        A_OK,
        F_NO_FLOOR,
        B_NO_FLOOR,
        WALL_BEHIND,
        S_LEFT,
        S_CENTER,
        S_RIGHT
      */
      //stopBlinking = true;
      //DEBUG_PRINTLN("====");
      pcf20.write(PCF_okLed, HIGH);
      pcf20.write(PCF_backingUpLed, LOW);
      if (hasChanged_fd)
      {
        problemState = F_NO_FLOOR;
#ifdef SERVO_ENABLE
        DEBUG_PRINTFLN("sweeper0", sweeper[0].GetPos());
#endif
        DEBUG_PRINTFLN("fd", hasChanged_fd);
      }
      if (hasChanged_bd)
      {
        problemState = B_NO_FLOOR;
#ifdef SERVO_ENABLE
        DEBUG_PRINTFLN("sweeper1", sweeper[1].GetPos());
#endif
        DEBUG_PRINTFLN("bd", hasChanged_bd);
      }
      if (hasChanged_bu && problemState != A_OK)
      {
        // CAN'T GO BACK
        problemState = WALL_BEHIND;
        emergStop = true;
        DEBUG_PRINTFLN("bu", hasChanged_bu);
      }
      //DEBUG_PRINTLN("-----");
    }
    sens_hit_flag = true;
  }
  else // no sensors have been flagged
  {
    sens_hit_flag = false;
    pcf20.write(PCF_backingUpLed, HIGH);
    //pcf20.write(PCF_okLed, LOW);
  }
}

// SONAR
void Sonar()
{
  // SONAR
  // Loop through all the sensors.
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    if (millis() >= pingTimer[i]) // Is it this sensor's time to ping?
    {
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      // Sensor ping cycle complete, do something with the results.
      if (i == 0 && currentSensor == SONAR_NUM - 1)
        oneSensorCycle();
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.
  // END SONAR
}

// If ping received, set the sensor distance to array.
void echoCheck()
{
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

// Sensor ping cycle complete, do something with the results.
void oneSensorCycle()
{
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    if (cm[i] && cm[i] < 1000 && problemState == A_OK)
    {
      if (cm[i] < 13)
      {
        pcf20.write(PCF_okLed, HIGH);
        pcf20.write(PCF_backingUpLed, LOW);
        switch (i)
        {
          /*
            A_OK,
            F_NO_FLOOR,
            B_NO_FLOOR,
            WALL_BEHIND,
            S_LEFT,
            S_CENTER,
            S_RIGHT
          */
          case 0: // left
            pcf20.write(PCF_backingUpLed, LOW);
            problemState = S_LEFT;
            break;
          case 1: // center
            pcf20.write(PCF_backingUpLed, LOW);
            problemState = S_CENTER;
            break;
          case 2: // right
            pcf20.write(PCF_backingUpLed, LOW);
            problemState = S_RIGHT;
            break;
        }
        //        DEBUG_PRINTF("i", i);
        //        DEBUG_PRINTFLN(" cm[i]", cm[i]);
      }
      //else        tooClose = false;
    }
  } // end for each sensor
  if (problemState == A_OK)
  {
    //pcf20.write(PCF_okLed, LOW);
    pcf20.write(PCF_backingUpLed, HIGH);
    //stopMe = false;
  }
}

/* MOTOR / H-BRIDGE */

void setMotor()
{
  analogWrite( mPins[curMotorNum][EN], curSpeed);
  digitalWrite(mPins[curMotorNum][IN_A], reverse[curMotorNum]);
  digitalWrite(mPins[curMotorNum][IN_B], !reverse[curMotorNum]);
}

void blink_(int ledP, int secs)
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillisBlink >= secs)
  {
    lastMillisBlink = currentMillis;
    pcf20.write(ledP, !pcf20.read(ledP));
  }
}

