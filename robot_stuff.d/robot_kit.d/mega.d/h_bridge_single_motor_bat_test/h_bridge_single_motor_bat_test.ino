#include <NS_energyShield.h>
#include <Wire.h>

#define DEBUG 1

NS_energyShield eS; // Create NS-energyShield object called "eS"

int Voltage, Current, AdapterVoltage;
float Percent, Temperature;

const byte enablePin = 11;
const byte in1Pin = 10;
const byte in2Pin = 9;
const byte switchPin = 7;

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
#ifdef DEBUG
  Serial.begin(9600);
  while(!Serial); // Wait for open COM port
  delay(5000);
  Serial.println("Voltage (mV)\tCurrent(mA)\tCharge (%)\tTemperature (C)\tAdapter Voltage (mV)"); // Prints header
#endif
}
 
void loop()
{
  int speed = 130; //analogRead(potPin) / 4;
  boolean reverse = digitalRead(switchPin);
#ifdef DEBUG
  Voltage = eS.voltage(); // Voltage is returned in mV
  Current = eS.current(); // Current is returned in mA
  Percent = (float) eS.percent()/2; // Percent is returned in 0.5% increments
  Temperature = (float) eS.temperature()/8; // Temperature is returned in 0.125 C increments
  AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage 
  Serial.print(Voltage);
  Serial.print(" mV\t\t");
  Serial.print(Current);
  Serial.print(" mA\t\t");
  Serial.print(Percent,1);
  Serial.print(" %\t\t");
  Serial.print(Temperature,2);
  Serial.print(" C\t\t");
  Serial.print(AdapterVoltage);
  Serial.println(" mV");
#endif
  setMotor(speed, reverse);
}
 
void setMotor(int speed, boolean reverse)
{
  analogWrite(enablePin, speed);
  digitalWrite(in1Pin, !reverse);
  digitalWrite(in2Pin, reverse);
}
