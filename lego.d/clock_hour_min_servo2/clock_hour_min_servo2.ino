// 0 - 4076 - stepper
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <Servo.h>
#include <AccelStepper.h>
#include <MyDelay.h>

MyDelay md(1000);

//#define SET_CLOCK

#define PENDULUM

#define AM 0
#define PM 1

#define HOUR_A 0
#define HOUR_B 1

#define MINUTE_A 0
#define MINUTE_B 1

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

Servo hourHand[2];
Servo minuteHand[2];

int myH, myM, myS;// myDa, myMo, myYe;
unsigned long lastMillis = 0;
int num = 0;

const int waitDelay = 1000;

const byte hPin[] = { 2, 3 };
const byte mPin[] = { 4, 5 };

int xH, mH;
int xM, mM;

int servo_hour;
int servo_minute;

boolean am_pm = AM;

boolean h_ab = HOUR_A;
boolean m_ab = MINUTE_A;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  for (int i = 0; i < 2; i++)
  {
    hourHand[i].attach(hPin[i]);
    minuteHand[i].attach(mPin[i]);
  }
  hourHand[1].write(177);
  minuteHand[1].write(177);
  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(140.0);
  stepper.setSpeed(700);
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef SET_CLOCK
  RTC.initClock();
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  RTC.setDate(25, 3, 4, 0, 17);
  //hr, min, sec
  RTC.setTime(9, 57, 00);
#endif
}

void loop()
{
  myH = RTC.getHour();
  myM = RTC.getMinute();
  if (md.check())
  {
    Serial.print(myH);
    Serial.print(F(":"));
    Serial.println(myM);
  }
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
  if (myM <= 30)
  {
    xM = 30;
    mM = 0;
    m_ab = MINUTE_A;
  }
  else
  {
    xM = 59;
    mM = 31;
    m_ab = MINUTE_B;
  }
  servo_hour = map(myH, mH, xH, 135, 15);
  servo_minute = map(myM, mM, xM, 135, 5);
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
  if (m_ab == MINUTE_A)
  {
    minuteHand[0].write(servo_minute);
    minuteHand[1].write(177);
  }
  else
  {
    minuteHand[0].write(177);
    minuteHand[1].write(servo_minute);
  }
  // waits for the servo to get there
  delay(15);

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
#ifdef PENDULUM
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
#else
  num = map(myM, 0, 59, 0, 4076);
  //if (stepper.distanceToGo() != 0) { }
  stepper.moveTo(num);
  stepper.run();
#endif
}

