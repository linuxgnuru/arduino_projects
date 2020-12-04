#include <AccelStepper.h>

#include <NS_energyShield.h>
#include <Wire.h>
#include <LiquidCrystal.h>

// Global Variables
int Current;//, AdapterVoltage;
float Voltage, Percent, Temperature, AdapterVoltage;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// Defines NS_energyShield object called "eS"
NS_energyShield eS;
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1 8     // IN1 on the ULN2003 driver 1
#define motorPin2 9     // IN2 on the ULN2003 driver 1
#define motorPin3 10    // IN3 on the ULN2003 driver 1
#define motorPin4 11    // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

unsigned long lastMillis = 0;

const byte lowBatLed = 13;
const byte deadBatLed = 12;

// for battery shield
unsigned long lastCheckBattTime;
int chCurrent[10];
boolean charging = false;

void setup()
{
  // eS.address(B00110110); // Can be used if address is changed from default (Advanced)
  lcd.begin(16, 2);
  checkBatt();
  stepper.setMaxSpeed(700.0);
  stepper.setAcceleration(140.0);
  stepper.setSpeed(400);
}

void loop()
{
  unsigned long curMillis = millis();

  if (curMillis - lastMillis >= 1000)
  {
    lastMillis = curMillis;
    Voltage = eS.voltage(); // Voltage is returned in mV
    Current = eS.current(); // Current is returned in mA
    Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
    Temperature = (float) eS.temperature() / 8; // Temperature is returned in 0.125 C increments
    AdapterVoltage = eS.Vadp(2); // Reads DC adapater input voltage
    lcd.setCursor(0, 0);
    lcd.print(Voltage / 1000);
    lcd.print(" ");
    lcd.print(Current);
    lcd.print(" ");
    lcd.print(Percent, 1);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("");
    lcd.print(Temperature, 2);
    lcd.print("C ");
    lcd.print(AdapterVoltage / 1000);
  }
  if (stepper.distanceToGo() == 0)
    stepper.moveTo(-stepper.currentPosition());
  stepper.run();
  checkBatt();
}

void checkBatt()
{
  unsigned long currentMillis = millis();

  if (currentMillis > lastCheckBattTime + 100)
    isCharging();
  if (currentMillis % 5000 == 0)
  {
    // don't test if we are charging
    if (!charging)
    {
      // internal battery
      Percent = (float) eS.percent() / 2; // Percent is returned in 0.5% increments
      digitalWrite(lowBatLed, (Percent < 50));
      digitalWrite(deadBatLed, (Percent < 20));
    }
    else
    {
      digitalWrite(lowBatLed, LOW);
      digitalWrite(deadBatLed, LOW);
    }
  }
}

void isCharging()
{
  charging = true;
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
  }
  lastCheckBattTime = millis();
  //if (charging)    DEBUG_PRINTLN("- charging -");
}

