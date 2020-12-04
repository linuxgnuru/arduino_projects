// sonic-alarm
/*
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
         VCC: +5VDC
        Trig: Trigger (INPUT) - Pin8
        Echo: Echo (OUTPUT) - Pin 9
         GND: GND
 */

#define IN 1
//#define CM 1

int mledPin = 10;
int pzPin = 11;  // piezo buzzer
int magPin = 7;
int trigPin = 8;    // Trig - green Jumper
int echoPin = 9;    // Echo - yellow Jumper
long duration, cm, inches;

int i;
 
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // turn off the 13 LED
#ifdef SER
    //Serial Port begin
  Serial.begin(9600);
#endif
  //Define inputs and outputs
  pinMode(pzPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(magPin, INPUT);
  pinMode(mledPin, OUTPUT);
  Serial.begin(9600);
}
 
void loop()
{
  Serial.println(analogRead(A0));
  digitalWrite(mledPin, (digitalRead(magPin) == HIGH ? HIGH : LOW));
  /*
  getDistance();
#ifdef IN
  digitalWrite(pzPin, (inches < 10 ? HIGH : LOW));
#else
  digitalWrite(pzPin, (cm < 20 ? HIGH : LOW));
#endif
#ifdef SER
  Serial.print(inches);
  Serial.print(" in, ");
  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();
#endif
*/
  delay(250);
}

void getDistance()
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
}
