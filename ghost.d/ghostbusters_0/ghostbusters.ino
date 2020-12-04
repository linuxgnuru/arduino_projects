#include <Servo.h>
#include <SPI.h>

Servo servo_left, servo_right;
const byte LATCH = 10;

byte ledLeft, ledRight;
byte ledGreen[] = { 0b00000000, 0b00000000 };

unsigned long lastMillis = 0;

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  servo_left.attach(5);
  servo_right.attach(6);
}

void loop()
{
  long val = analogRead(A0);
  int d = map(val, 0, 1023, 0, 180);
  unsigned long curMillis = millis();
  for (int i = 0; i < d; i += 15)
  {
    if (curMillis - lastMillis >= 100)
    {
      lastMillis = curMillis;
      servo_left.write(i);
      servo_right.write(i);
    }
  }
}

// TODO 

void writeMap()
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(ledGreen[0]);
  SPI.transfer(ledGreen[1]);
  SPI.transfer(ledRight);
  SPI.transfer(ledLeft);
  digitalWrite(LATCH, HIGH);
}

void writeLED(long v)
{
  int m = map(v, 0, 1023, 0, 6);
  switch (m)
  {
    case 0: ledRight = ledLeft = 0b00000001; break;
    case 1: ledRight = ledLeft = 0b00000011; break;
    case 2: ledRight = ledLeft = 0b00000111; break;
    case 3: ledRight = ledLeft = 0b00001111; break;
    case 4: ledRight = ledLeft = 0b00011111; break;
    case 5: ledRight = ledLeft = 0b00011111; break;
    case 6: ledRight = ledLeft = 0b00111111; break;
  }
  digitalWrite(LATCH, LOW);
  SPI.transfer(ledGreen[0]);
  SPI.transfer(ledGreen[1]);
  SPI.transfer(ledRight);
  SPI.transfer(ledLeft);
  digitalWrite(LATCH, HIGH);
}

#if 0
// EMF Detector for 7-Segment LED Display v1.0
//
// original code/project by Aaron ALAI - aaronalai1@gmail.com
// modified for use w/ LED bargraph by Collin Cunningham - collin@makezine.com
// modified again by ComputerGeek for instructable & 7-segment display.

#define NUMREADINGS 15 // raise this number to increase data smoothing

int senseLimit = 15; // raise this number to decrease sensitivity (up to 1023 max)
int probePin = 5; // analog 5
int val = 0; // reading from probePin

// variables for smoothing

int readings[NUMREADINGS];                // the readings from the analog input
int index = 0;                            // the index of the current reading
int total = 0;                            // the running total
int average = 0;                          // final average of the probe reading

//CHANGE THIS TO affect the speed of the updates for numbers. Lower the number the faster it updates.
int updateTime = 40;

void setup()
{
  Serial.begin(9600);  // initiate serial connection for debugging/etc
  for (int i = 0; i < NUMREADINGS; i++)
    readings[i] = 0;                      // initialize all the readings to 0
}

void loop()
{
  val = analogRead(probePin);  // take a reading from the probe

  // if the reading isn't zero, proceed
  if (val >= 1)
  {
    // turn any reading higher than the senseLimit value into the senseLimit value
    val = constrain(val, 1, senseLimit);
    // remap the constrained value within a 1 to 1023 range
    val = map(val, 1, senseLimit, 1, 1023);
    // subtract the last reading
    total -= readings[index];
    // read from the sensor
    readings[index] = val;
    total += readings[index];               // add the reading to the total
    index = (index + 1);                    // advance to the next index
    if (index >= NUMREADINGS)               // if we're at the end of the array...
      index = 0;                            // ...wrap around to the beginning
    average = total / NUMREADINGS;          // calculate the average
    if (average > 50)
      showLED0();
    if (average > 150)
      showLED1();
    if (average > 250)
      showLED2();
    if (average > 350)
      showLED3();
    if (average > 450)
      showLED4();
    if (average > 550)
      showLED5();
    if (average > 650)
      showLED6();
    if (average > 750)
      showLED7();

    if (average > 850)
      showLED8();
    if (average > 950)
      showLED9();
    Serial.println(average); // use output to aid in calibrating
    delay(updateTime);
  }

}
#endif

