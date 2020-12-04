/*
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND
 */
 
int trigPin = 8;    // Trig - Yellow Jumper
int echoPin = 9;    // Echo - Orange Jumper
int relayPin = 7;
int belowPin = 10;
long duration;
int cm, inches;
int threshHold = 6;
int backwardsTime = 2000;

int i;
 
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(belowPin, INPUT);
}
 
void loop()
{
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
  if (inches < threshHold || digitalRead(belowPin) == LOW)
  {
    digitalWrite(relayPin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(backwardsTime);
    digitalWrite(relayPin, LOW);
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(250);
}

