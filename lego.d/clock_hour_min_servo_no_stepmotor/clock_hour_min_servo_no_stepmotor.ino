#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <Servo.h>

//#define SET_CLOCK

#define AM 0
#define PM 1

#define HOUR_A 0
#define HOUR_B 1

#define MINUTE_A 0
#define MINUTE_B 1

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
  Wire.begin();
  for (int i = 0; i < 2; i++)
  {
    hourHand[i].attach(hPin[i]);
    minuteHand[i].attach(mPin[i]);
  }
  hourHand[1].write(177);
  minuteHand[1].write(177);
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
}

