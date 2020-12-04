/*
   robot in all it's glory

   what I want it to do
   - move forward
   - check 3 sonar sensors to see if it should stop moving forward
   - if left one
*/
#include <NS_energyShield.h>
#include <Wire.h>
#include <PCF8574.h>
#include <NewPing.h>

/* BEGIN pcf8574 stuff */

PCF8574 pcf20(0x20);

#define ALL_OK    0
#define BOT_FRONT 1
#define BOT_BACK  2
#define TOP_BACK  4

const byte frontDownSensorPin = 0;
const byte backDownSensorPin  = 1;
const byte backUpSensorPin  = 2;

const byte runningOKLed = 3;
const byte lowBatIntLed = 4;
const byte lowBatExtLed = 5;
const byte backingUpLed = 6;

unsigned int lastMillis;

volatile boolean hasChanged = false;

boolean hasChanged_fd = false;
boolean hasChanged_bd = false;
boolean hasChanged_bu = false;

/* END pcf8574 stuff */

/* BEGIN Battery stuff */
#define BATT_CAP 1200 // mAh
#define NUM_SAMPLES 10

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
boolean first_time = true;

typedef enum {
  INTERNAL_BATT,
  EXTERNAL_BATT
} battery;

/* END Battery stuff */

/* BEGIN h-bridge stuff */
const byte h_pins[] = {
  
};
const byte en1pin = 3;
const byte in1pin = 4;
const byte in2pin = 5;

const byte en2pin = 6;
const byte in3pin = 7;
const byte in4pin = A2;

const byte threshHold = 3;
const int backTime = 2000;
unsigned long previousMillis = 0;

typedef enum {
  STOP,
  FORWARD,
  BACKUP
} direction_enum;

/* END h-bridge stuff */

/* BEGIN sonar stuff */

#define SONAR_NUM      3 // Number of sensors.
#define MAX_DISTANCE  50 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing( 8,  9, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(10, 11, MAX_DISTANCE),
  NewPing(12, LED_BUILTIN, MAX_DISTANCE)
};

/* END sonar stuff */

void setup()
{
  // for debuging
  Serial.begin(115200);
  // pcf8574
  pcf20.begin();
  // sonar
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  // end sonar
  // hbridge
  for (int i = 0; i < 6; i++)
    pinMode(h_pins[i], OUTPUT);
  pinMode(in4pin, OUTPUT);
  // end hbridge
  //  attachInterrupt(0, catchChange, CHANGE);
  attachInterrupt(0, catchChange, FALLING);
}

void loop()
{
  unsigned int currentMillis = millis();
  if (currentMillis - lastMillis >= 50)
  {
    lastMillis = currentMillis;
    if (hasChanged)
    {
      hasChanged_fd = !pcf20.readButton(frontDownSensorPin);
      hasChanged_bd = !pcf20.readButton(backDownSensorPin);
      hasChanged_bu = !pcf20.readButton(backUpSensorPin);
      if (hasChanged_fd || hasChanged_bd || hasChanged_bu)
      {
        pcf20.write(runningOKLed, HIGH);
        if (hasChanged_fd)
        {
          pcf20.write(backingUpLed, LOW);
        }
        if (hasChanged_bd)
        {
          pcf20.write(backingUpLed, LOW);
        }
        if (hasChanged_bu)
        {
          pcf20.write(backingUpLed, LOW);
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
  checkBatt();
}

void checkSonar()
{
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
  if (cm[0] < 40) // left sensor
  {
    Serial.println(F("left"));
  }
  if (cm[1] < 40) // center sensor
  {
    Serial.println(F("center"));
  }
  if (cm[2] < 40) // right sensor
  {
    Serial.println(F("right"));
  }
#if 0
  // The following code would be replaced with your code that does something with the ping results.
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    //Serial.print(i);
    //Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("\t");
    //Serial.print("cm ");
  }
  Serial.println();
#endif
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

void checkBatt()
{
  Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
  pcf20.write(lowBatIntLed, !(Percent < 30));
  //if (millis() > lastTime + 100)
    myVin = getVolt(voltPin);
  pcf20.write(lowBatExtLed, !(myVin < 4));//!((millis() % 3000 == 0) && myVin < 4));
  pcf20.write(runningOKLed, ((Percent < 30) || (myVin < 4)));
  delay(1);
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
  sample_count = 0;
  sum = 0;
  // not sure why it's off by 2
  vin -= 2;
  if (vin < 0.09)
    vin = 0.0;
  lastTimeB = millis();
  return vin;
}

/*
   pcf8574 stuff
*/
void catchChange()
{
  hasChanged = true;
}

/*
   h-bridge/motor stuff
*/
#if 0
void loop()
{
  for (int i = 0; i < 3; i++)
  {
    if (testBlock(i))
    {
      switch (i)
      {
        case 0: // left of robot
          // left, right, delay, speed (0 - 255)
          // 0 = stop, 1 = forward, -1 = reverse
          runMotor(0, -1, backTime, 255);
          break;
        case 1: // center
          runMotor(-1, -1, backTime, 255);
          break;
        case 2: // right
          runMotor(-1, 0, backTime, 255);
          break;
        default:
          // never should get to this code.
          runMotor(0, 0, backTime, 255);
          break;
      }
    }
    else
    {
      runMotor(1, 1, 0, 255);
    }
  }
}

void runMotor(int left, int right, int dur, int speed)
{
  unsigned long currentMillis = millis();
  switch (left)
  {
    case -1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite( en1pin, speed);
          digitalWrite(in1pin, LOW);
          digitalWrite(in2pin, HIGH);
        }
      }
      break;
    case 0:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite( en1pin, speed);
          digitalWrite(in1pin, LOW);
          digitalWrite(in2pin, LOW);
        }
      }
      break;
    case 1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite( en1pin, speed);
          digitalWrite(in1pin, HIGH);
          digitalWrite(in2pin, LOW);
        }
      }
      break;
  }
  switch (right)
  {
    case -1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite( en2pin, speed);
          digitalWrite(in3pin, LOW);
          digitalWrite(in4pin, HIGH);
        }
      }
      break;
    case 0:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite( en2pin, speed);
          digitalWrite(in3pin, LOW);
          digitalWrite(in4pin, LOW);
        }
      }
      break;
    case 1:
      if (currentMillis - previousMillis >= dur)
      {
        if (!testBlock(4))
        {
          // save the last time you blinked the LED
          previousMillis = currentMillis;
          analogWrite( en2pin, speed);
          digitalWrite(in3pin, HIGH);
          digitalWrite(in4pin, LOW);
        }
      }
      break;
  }
}
#endif

/*
   sonar stuff
*/
// ---------------------------------------------------------------------------
// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------

