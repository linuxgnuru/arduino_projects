#include <NS_energyShield.h>
#include <PCF8574.h>
#include <Wire.h>
#include <NewPing.h>

#define DEBUG
//#define SENSOR_TEST
//#define INT_BAT_DEBUG
//#define EXT_BAT_DEBUG
//#define NO_EXT_CHECK
// #define SONAR_ENABLE
//#define SONAR_DEBUG
#define NO_BAT_CHECK

#ifdef NO_BAT_CHECK
#ifndef NO_EXT_CHECK
#define NO_EXT_CHECK
#endif
#endif

#include <DebugUtils.h>

// The following was a test to
/*
  typedef enum {
  ALL_OK = 0,
  DOWN_F = 1,
  DOWN_B = 2,
  BACK_B = 4
  } status_enum;
*/

// To be used later; might just make it boolean
#define MOTOR_A 0
#define MOTOR_B 1

/* SONAR */
#define SONAR_NUM     3 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

NS_energyShield eS; // Create NS-energyShield object called "eS"
PCF8574 pcf20(0x20);

boolean sens_hit_flag = false;

/* BATTERY */

// for internal battery charging test
long lastCheckTime = millis(); // void checkBatt()
int chCurrent[10]; // void isCharging()
boolean charging;
int Voltage, Current, AdapterVoltage;
float Temperature, Percent; // temperature and percentage of internal battery remaining

float myVin = 0.0; // loop()

boolean extBatAvail = true;

/* END BATTERY */

/* BEGIN PCF8574 and sensors */

const byte frontDownSensorPin = 0;
const byte backDownSensorPin  = 1;
const byte backUpSensorPin    = 2;

const byte runningOKLed = 4; // led - green
const byte lowBatIntLed = 5; // led - yellow
const byte lowBatExtLed = 6; // led - red
const byte backingUpLed = 7; // led - blue

volatile boolean hasChanged = false;

boolean hasChanged_forward_down = false;
boolean hasChanged_back_down    = false;
boolean hasChanged_back_back    = false;

/* END PCF8574 */

/* BEGIN timing counters */

unsigned int lastMillisInput_PCF = 0;
unsigned int lastMillisBatt = 0;
unsigned int lastMillisBlink = 0;
unsigned int lastMillisBlinkBlue = 0;
unsigned int lastMillisCB = 0;
unsigned int lastMillisDEBUG = 0;
unsigned int lastMillisBlinkKill = 0;
unsigned int lastMillisV = 0;

/* SONAR */

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

// Sensor object array.
// Each sensor's trigger pin, echo pin, and max distance to ping.
NewPing sonar[SONAR_NUM] = {
  NewPing(12, LED_BUILTIN, MAX_DISTANCE), // (facing front) left
  NewPing(10, 11, MAX_DISTANCE), // center
  NewPing( 8,  9, MAX_DISTANCE)  // right
};

/* H BRIDGE */
typedef enum {
  EN,
  IN_A,
  IN_B
} pin_enum;


// PINS
const byte voltPin = A2; // for checking external battery voltage

// to use this method:  mPins[MOTOR_RIGHT][EN]
const byte mPins[2][3] = {
  // EN  IN_A IN_B
  {  6, A1,  A3 }, // motor 0 (right)
  {  5,  4,   3 }  // motor 1 (left)
};

boolean emergStop = false;

// --- IMPORTANT variables / flags ---

boolean backup_back,
        backup_left,
        backup_right,
        backup_back_left;

void setup()
{
  // h-bridge
  for (int i = 0; i < 3; i++)
  {
      pinMode(mPins[MOTOR_A][i], OUTPUT);
      pinMode(mPins[MOTOR_B][i], OUTPUT);
  }
  pcf20.begin();
#ifdef DEBUG
  Serial.begin(9600);
#ifdef INT_BAT_DEBUG
  while (!Serial); // Wait for open COM port
  delay(5000);
  //  DEBUG_PRINTLN("Voltage (V)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (V)\tCharging");
  DEBUG_PRINTLN("Voltage (V)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (V)");
#endif
#endif
  // SONAR
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  //attachInterrupt(0, catchChange, FALLING);
  attachInterrupt(0, catchChange, CHANGE);
  pcf20.write(runningOKLed, LOW);
#ifndef NO_EXT_CHECK
  checkBatt();
#endif
}

void loop()
{
  unsigned int currentMillis = millis();
  /*
     H-bridge:
  */
#ifdef INT_BAT_DEBUG
  Voltage = eS.voltage(); // Voltage is returned in mV
  Current = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature() / 8; // Temperature is returned in 0.125 C increments
  AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage
  if (currentMillis - lastMillisV >= 1000)
  {
    lastMillisV = currentMillis;
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
  // SONAR
  // Loop through all the sensors.
#ifdef SONAR_ENABLE
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
#endif
  // TODO FIXME XXX since we are using interrupts, we might not need this test...

  // Limit button read to 20 times a second.  Fast enough for most buttons but
  // this way you don't have a dimmer output because it's blanked during button
  // read a read takes 460us t 16Mhz Arduino and normal I2C speed.
  if (currentMillis - lastMillisInput_PCF >= 50)
  {
    lastMillisInput_PCF = currentMillis;
    if (hasChanged)
    {
      // don't do this test if we are debuging
#ifndef DEBUG
      // the front and back will show 0/LOW/false if everything is ok
      hasChanged_forward_down = pcf20.readButton(frontDownSensorPin);
      hasChanged_back_down = pcf20.readButton(backDownSensorPin);
      // the behind sensor gives 1/HIGH/true if everything is ok
      hasChanged_back_back = !pcf20.readButton(backUpSensorPin);
#else
      hasChanged_forward_down = false;
      hasChanged_back_down = false;
      hasChanged_back_back = false;
#endif
      if (hasChanged_forward_down || hasChanged_back_down || hasChanged_back_back)
      {
        if ((hasChanged_forward_down && hasChanged_back_down) || (hasChanged_forward_down && hasChanged_back_down && hasChanged_back_back))
        {
          DEBUG_PRINTLN("-=-=-= ERROR =-=-=-");
          blinkLeds();
        }
        else
        {
          pcf20.write(backingUpLed, LOW);
          pcf20.write(runningOKLed, HIGH);
#ifdef SENSOR_TEST
          DEBUG_PRINTLN("====");
          if (hasChanged_forward_down)
          {
            DEBUG_PRINTFLN("forward_down", hasChanged_forward_down);
          }
          if (hasChanged_back_down)
          {
            DEBUG_PRINTFLN("back_down", hasChanged_back_down);
          }
          if (hasChanged_back_back)
          {
            DEBUG_PRINTFLN("back_back", hasChanged_back_back);
          }
          DEBUG_PRINTLN("-----");
#endif
        }
        sens_hit_flag = false;
      }
      else // no sensors have been flagged
      {
        if (!sens_hit_flag)
        {
          //DEBUG_PRINTLN("no sensors hit");
          sens_hit_flag = true;
        }
        pcf20.write(runningOKLed, LOW);
        pcf20.write(backingUpLed, HIGH);
      }
      hasChanged = false;
    } // end hasChanged
  } // end millis test
//#if !defined NO_BAT_CHECK || !defined NO_EXT_CHECK
#ifndef NO_EXT_CHECK
  // blink the low external battery led if no battery is detected
  if (!extBatAvail)
  {
    blink_extBat();
  }
#endif
  // only check the batteries every 3 minutes and then, only if we have to stop or something.
  //if (currentMillis - lastMillisB >= 180000 && (pcf20.read(backingUpLed)))
  // check battery status every 3 seconds
#ifndef NO_BAT_CHECK
  if (currentMillis - lastMillisBatt >= 3000)
  {
    //DEBUG_PRINTLN("checking batteries");
    lastMillisB = currentMillis;
    checkBatt();
  }
#endif
}

/* BEGIN BATTERY */
void checkBatt()
{
  unsigned int currentMillis = millis();
  // check to see if we are charging
  if (currentMillis > lastCheckTime + 100)
    isCharging();
  if (currentMillis % 5000 == 0)
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
      pcf20.write(runningOKLed, LOW);
    }
  }
#ifndef NO_EXT_CHECK
  // external battery
  if (extBatAvail)
  {
    myVin = getVolt(voltPin);
    if (myVin > 3)
    {
      pcf20.write(lowBatExtLed, !(myVin < 4));//!((millis() % 3000 == 0) && myVin < 4));
      pcf20.write(runningOKLed, ((!charging) && ((Percent < 30) || (myVin < 3))));
    }
    else
    {
      extBatAvail = false;
    }
  }
  else // no battery was found
  {
    // turn off the running ok led if the percent is under 30
    pcf20.write(runningOKLed, ((!charging) && (Percent < 30)));
    pcf20.write(lowBatIntLed, !((!charging) && (Percent < 30)));
  }
#else
  pcf20.write(lowBatExtLed, HIGH);
  pcf20.write(lowBatIntLed, HIGH);
  extBatAvail = true;
#endif
}

float getVolt(int aPin)
{
  float vin = 0.0;
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
  // DELAY
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

// TODO / FIXME the following isn't working correctly for some reason...
// both front and back sensors say there's nothing under them or every sensor is triggered
void blinkLeds()
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillisBlinkBlue >= 100)
  {
    lastMillisBlinkBlue = currentMillis;
    DEBUG_PRINTLN("blink blue");
    pcf20.write(backingUpLed, !pcf20.read(backingUpLed));
    //    pcf20.write(runningOKLed, !pcf20.read(runningOKLed));
  }
}

void blink_extBat()
{
  unsigned int currentMillis = millis();
  // only blink for 3 minutes
  //if (currentMillis - lastMillisBlinkKill >= 180000)
  if (currentMillis - lastMillisBlinkKill >= 30000)
  {
    lastMillisBlinkKill = currentMillis;
    pcf20.write(lowBatExtLed, LOW);
#ifndef INT_BAT_DEBUG
    DEBUG_PRINTLN("3 minutes has past!!");
#endif
  }
  else // don't blink the led if it's been over 3 minutes
  {
    if (currentMillis - lastMillisBlink >= 1000)
  {
    lastMillisBlink = currentMillis;
    pcf20.write(lowBatExtLed, !pcf20.read(lowBatExtLed));
  }
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
      charging = false;
    }
    else
      charging = true;
  }
  lastCheckTime = millis();
  //if (charging)    DEBUG_PRINTLN("- charging -");
}

// SONAR

// If ping received, set the sensor distance to array.
void echoCheck()
{
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

byte flag = 0;

// Sensor ping cycle complete, do something with the results.
void oneSensorCycle()
{
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    if (cm[i])
    {
      if (cm[i] < 5)
      {
        switch (i)
        {
          case 0: // left
            pcf20.write(backingUpLed, LOW);
            pcf20.write(lowBatIntLed, LOW);
            break;
          case 1: // center
            pcf20.write(runningOKLed, HIGH);
            pcf20.write(backingUpLed, LOW);
            pcf20.write(lowBatIntLed, LOW);
            pcf20.write(lowBatExtLed, LOW);
            break;
          case 2: // right
            pcf20.write(backingUpLed, LOW);
            pcf20.write(lowBatIntLed, HIGH);
            pcf20.write(lowBatExtLed, LOW);
            break;
          default:
            break;
        }
      }
      else
      {
        pcf20.write(runningOKLed, LOW);
        pcf20.write(backingUpLed, HIGH);
        pcf20.write(lowBatIntLed, HIGH);
        pcf20.write(lowBatExtLed, HIGH);
      }
    }
    delay(1);
#ifndef SONAR_DEBUG
  }
#else
    if (cm[i])
    {
      DEBUG_PRINT(i);
      DEBUG_PRINT("=");
      DEBUG_PRINT(cm[i]);
      DEBUG_PRINTLN("cm\t");
      flag = 1;
    }
  }
  if (flag)
  {
    DEBUG_PRINTLN("");
    flag = 0;
  }
#endif
}

/* MOTOR / H-BRIDGE */

void runMotor()
{
  int speed = 127;
  boolean reverse = false;

  setMotor(0, speed, reverse);
  setMotor(1, speed, reverse);
}

void setMotor(int motorNum, int speed, boolean reverse)
{
  analogWrite(mPins[motorNum][EN], speed);
  digitalWrite(mPins[motorNum][IN_A], reverse);
  digitalWrite(mPins[motorNum][IN_B], !reverse);
}

/*
   Taken from sonar_test:

   ---------------------------------------------------------------------------
   This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
   the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
   "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
   is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
   are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
   would normally process the sensor results in this function (for example, decide if a robot needs to
   turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
   to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
   processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
   ---------------------------------------------------------------------------
*/

