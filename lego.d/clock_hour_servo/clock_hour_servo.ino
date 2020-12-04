// 0 - 4076 - stepper
#define AM 0
#define PM 1

#define HOUR_A 0
#define HOUR_B 1

#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#include <AccelStepper.h>

// stepper motor for pendulum
#define HALFSTEP 8

// Motor pin definitions
#define motorPin1   8     // IN1 on the ULN2003 driver 1
#define motorPin2   9     // IN2 on the ULN2003 driver 1
#define motorPin3  10     // IN3 on the ULN2003 driver 1
#define motorPin4  11     // IN4 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);

Rtc_Pcf8563 RTC;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

Servo hourHand[2];
Servo minuteHand[2];

int myH, myM, myS;//, myDa, myMo, myYe;
unsigned long lastMillis = 0;
int num = 0;

const int waitDelay = 1000;
const byte hPin[] = { A3, A2 };
//const byte mPin[] = { 10, 11 };

int xH, mH;
//int xM, mM;
int servo_hour;
//int servo_minute;

boolean h_ab = HOUR_A;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16, 2);
  for (int i = 0; i < 2; i++)
  {
    hourHand[i].attach(hPin[i]);
    //minuteHand[i].attach(mPin[i]);
  }
  hourHand[1].write(177);
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(140.0);
  stepper.setSpeed(700);
  pinMode(LED_BUILTIN, OUTPUT);
  /*
    RTC.initClock();
    RTC.setDate(3, 2, 4, 0, 17);
    RTC.setTime(7, 34, 00);
  */

}

boolean am_pm = AM;

void loop()
{
  lcd.setCursor(0, 0);
  myH = RTC.getHour();
  myM = RTC.getMinute();
  lcd.print(RTC.formatTime());
  lcd.setCursor(0, 1);
  lcd.print(RTC.formatDate());
  am_pm = (myH >= 12);
  myH = (myH > 12 || myH == 0 ? abs(myH - 12) : myH);
  if (myH <= 6)
  {
    xH = 6;
    mH = 1;
    h_ab = HOUR_A;
  }
  else
  {
    xH = 12;
    mH = 7;
    h_ab = HOUR_B;
  }
  /*
    if (myM <= 30)
    {
    xM = 30;
    mM = 0;
    }
    else
    {
    xM = 59;
    mM = 31;
    }
  */
  servo_hour = map(myH, mH, xH, 135, 15);
  //servo_minute = map(myM, mM, xM, 135, 5);
  if (h_ab == HOUR_A)
  {
    hourHand[0].write(servo_hour);
    hourHand[1].write(177);
  }
  else
  {
    hourHand[0].write(177);
    hourHand[1].write(servo_hour);
  }
  delay(15);                           // waits for the servo to get there

  /*
    DateTime now = RTC.now();
    myHo = now.hour();
    myMi = now.minute();
    mySe = now.second();
    myDa = now.day();
    myMo = now.month();
    myYe = now.year();
    if (myHo < 10)
    lcd.print('0');
    lcd.print(myHo);
    lcd.print(':');
    if (myMi < 10)
    lcd.print('0');
    lcd.print(myMi);
    lcd.print(':');
    if (mySe < 10)
    lcd.print('0');
    lcd.print(mySe);
    lcd.setCursor(0, 1);
    if (myDa < 10)
    lcd.print('0');
    lcd.print(myDa);
    lcd.print('/');
    if (myMo < 10)
    lcd.print('0');
    lcd.print(myMo);
    lcd.print('/');
    lcd.print(myYe);
  */
#if PENDULUM
  //stepper.runToNewPosition(500);
  //stepper.runToNewPosition(-500);

  stepper.moveTo(num);
  stepper.run();
  if (stepper.distanceToGo() == 0)
  {
    if (num == 0 || num == -500)
    {
      num = 500;
    }
    else if (num == 500)
    {
      num = -500;
    }
  }
#endif
  num = map(myM, 0, 59, 0, 4076);
  //if (stepper.distanceToGo() != 0) { }
  stepper.moveTo(num);
  stepper.run();
  /*
  Serial.print(myM);
  Serial.print(F("\t"));
  Serial.println(num);
  delay(250);
  */
}

