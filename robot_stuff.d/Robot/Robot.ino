/*
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
 Ultrasonic sensor Pins:
 VCC: +5VDC
 Trig : Trigger (INPUT) - Pin11
 Echo: Echo (OUTPUT) - Pin 12
 GND: GND
 */
int botLed = 2;
int sonarLed = A2;

int relayPin = A0;
int stopStartPin = A1;

int sonarPinGnd = 12; // ground; Keep low
int echoPin = 11;    // Echo - Orange Jumper
int trigPin = 10;    // Trig - Yellow Jumper
int sonarPinVcc = 9; // power; Keep high to turn on sonar

int belowPinVcc = 8;   // power to bottom sensor (keep high)
int belowPinSignal = 7;
int belowPinGnd = 6;    // ground (keep low)

int backupPinVcc = 5;   // power to backup sensor (keep high)
int backupPinSignal = 4;
int backupPinGnd = 3;    // ground (keep low)

// the following is to replace the delay
int relayState = LOW;
long previousMillis = 0;
long interval = 3000;

long duration;
int cm, inches;
int threshHold = 20;
//int backwardsTime = 2000;

int i;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef LED
  pinMode(botLed, OUTPUT);
  pinMode(sonarLed, OUTPUT);
#endif
  pinMode(relayPin, OUTPUT);
  pinMode(stopStartPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(backupPinSignal, INPUT);
  pinMode(belowPinSignal, INPUT);

  pinMode(sonarPinVcc, OUTPUT);
  pinMode(belowPinVcc, OUTPUT);
  pinMode(backupPinVcc, OUTPUT);

  pinMode(sonarPinGnd, OUTPUT);
  pinMode(backupPinGnd, OUTPUT);
  pinMode(belowPinGnd, OUTPUT);

  digitalWrite(sonarPinGnd, LOW);
  digitalWrite(belowPinGnd, LOW);
  digitalWrite(backupPinGnd, LOW);

  digitalWrite(sonarPinVcc, HIGH);
  digitalWrite(belowPinVcc, HIGH);
  digitalWrite(backupPinVcc, HIGH);
  digitalWrite(stopStartPin, HIGH);
}

void loop()
{
  unsigned long currentMillis = millis();
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  // convert the time into a distance
  cm = duration / 58; //(duration/2) / 29.1;
  inches = (duration/2) / 74; // 37
  if (digitalRead(backupPinSignal) == LOW)
  {
    digitalWrite(stopStartPin, LOW);
  }
  if ((digitalRead(backupPinSignal) == HIGH) && (cm < threshHold || digitalRead(belowPinSignal) == HIGH))
  {
#ifdef LED
    if (cm < threshHold)
    {
      digitalWrite(sonarLed, HIGH);
      digitalWrite(botLed, LOW);
    }
    else
    {
      digitalWrite(sonarLed, LOW);
      digitalWrite(botLed, HIGH);
    }
#endif
    digitalWrite(stopStartPin, HIGH);
    digitalWrite(relayPin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if ((digitalRead(backupPinSignal) == LOW) || (currentMillis - previousMillis > interval))
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(relayPin, LOW);
    previousMillis = currentMillis;
    if (digitalRead(backupPinSignal) == LOW)
      digitalWrite(stopStartPin, LOW);
    else
      digitalWrite(stopStartPin, HIGH);
  }
  delay(250);
}
/*
int powerPin = LED_BUILTIN;
 int sensorPin = 12;
 int relayPin = A0;
 
 void setup()
 {
 pinMode(sensorPin, INPUT);
 pinMode(relayPin, OUTPUT);
 pinMode(powerPin, OUTPUT);
 digitalWrite(powerPin, HIGH);
 }
 
 void loop()
 {
 if (digitalRead(sensorPin) == LOW)
 {
 digitalWrite(relayPin, HIGH);
 delay(3000);
 digitalWrite(relayPin, LOW);
 }
 else delay(100);
 }
 */






