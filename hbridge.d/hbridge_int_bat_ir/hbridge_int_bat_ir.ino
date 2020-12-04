#include <PCF8574.h>
#include <NS_energyShield.h>
#include <IRremote.h>

#include "vars.h"

#define SPEED_MAX 255
#define SPEED_MIN 190
// how long to wait for motor to reach target turn
#define WAIT_DL 20000

// NOTE: don't define DEBUG as the MPU6050 stuff does things.
//#define _WIGG_DEBUG
#define SHOW_LEDS

const byte PCF_leds[] = {
  //  0, 6, 7, 5, 4, 3, 2, 1 // clockwise
  //  0, 1, 2, 3, 4, 5, 7, 6 // counter clockwise
  0, 1, 2, 3, 4, 5, 7, 6 // counter clockwise
};

//  I1 E  I2
const byte motorPins[2][3] = {
  { 3, 5, 4 }, // motor 0 right
  { 7, 6, 8 }  // motor 1 left
};

const byte RECV_PIN = A1;
const byte voltPin = A3;

const byte internal_low_bat_led = 9;
const byte external_low_bat_led = 10;
const byte internal_dead_bat_led = 11;
const byte external_dead_bat_led = 12;

PCF8574 pcf20(0x20);
NS_energyShield eS; // Create NS-energyShield object called "eS"
IRrecv irrecv(RECV_PIN);

void setup()
{
  irrecv.enableIRIn(); // Start the receiver
  for (int i = internal_low_bat_led; i < external_dead_bat_led + 1; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = FORWARD; i < STOP; i++)
    lastDirMillis[i] = lastDebugMillis[i] = 0;
  Serial.begin(115200);
#ifdef _WIGG_DEBUG
  Serial.println(F("--- START ---"));
#endif
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 3; i++)
  {
    // h-bridge
    pinMode(motorPins[0][i], OUTPUT);
    pinMode(motorPins[1][i], OUTPUT);
  }
  pcf20.begin();
#ifdef SHOW_LEDS
  for (int i = 0; i < 8; i++)
  {
    pcf20.write(PCF_leds[i], !pcf20.read(PCF_leds[i]));
    delay(150);
  }
  for (int i = 9; i < 13; i++)
  {
    digitalWrite(i, !digitalRead(i));
    delay(150);
  }
  for (int i = 12; i > 8; i--)
  {
    digitalWrite(i, !digitalRead(i));
    delay(150);
  }
  for (int i = 7; i > -1; i--)
  {
    pcf20.write(PCF_leds[i], !pcf20.read(PCF_leds[i]));
    delay(150);
  }
#endif
  checkBatt();
}

boolean donef = false;

void loop()
{
  unsigned long currentMillis = millis();
  int dir;

  if (!extBatAvail)
    blink_d(external_dead_bat_led, 1000);
  if (currentMillis - lastMillisBat >= 3000)
  {
    lastMillisBat = currentMillis;
    checkBatt();
  }
  // checkIR();
  if (!stopMe && extBatAvail && !emergStop)
  {
    dir = checkIR();
    moveRobot(dir, (dir == FORWARD ? 5000 : (dir == STOP ? 1 : 250)));
  }
  /*
    if (demoMode)
    {
    }
    else
    {
      if (finalStop == false && !fubar)
      {
        if (cur_dir >= STOP)
        {
          if (ctr + 1 < 3)
          {
            cur_dir = FORWARD;
            ctr++;
          }
          else
            finalStop = true;
        }
        if (finalStop == false)
          moveRobot(cur_dir++, 250);
      }
      if (finalStop)
      {
        donef = true;
        moveRobot(STOP, 1);
        blink_p(FORWARD_LEFT, 1000);
        blink_p(FORWARD_RIGHT, 500);
        blink_p(BACKWARD_LEFT, 1000);
        blink_p(BACKWARD_RIGHT, 500);
      }
      if (fubar && !donef && !finalStop)
      {
        blink_p(LEFT, 500);
        blink_p(RIGHT, 500);
      }
    }
  */
  // just stop everything if no external battery
  if (!extBatAvail)
  {
    Snafu();
  }
}

/* MOTOR / H-BRIDGE */
void moveRobot(int dir, unsigned long dl)
{
  unsigned long currentMillis = millis();
  int ec;
  boolean bFlag = false;
  int i;

  if (!fubar)
  {
    switch (dir)
    {
      ////////////////////
      //     FORWARD    //
      ////////////////////
      case FORWARD:
        pcf20.write(PCF_leds[FORWARD], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          eSpeed[MOTOR_R] = SPEED_MAX; // 0 - 255; might need some testing to see what valid values actually work
          bridge[MOTOR_R][I1] = 1;
          bridge[MOTOR_R][I2] = 0;
          eSpeed[MOTOR_L] = SPEED_MAX; // 0 - 255; might need some testing to see what valid values actually work
          bridge[MOTOR_L][I1] = 0;
          bridge[MOTOR_L][I2] = 1;
          runMotor();
          currentMillis = millis();
#ifdef _WIGG_DEBUG
          Serial.print(F("dl: ")); Serial.print(dl);
          Serial.print(F(" lastFM: ")); Serial.print(lastForwardMillis);
          Serial.print(F(" cur - lfm: ")); Serial.println(currentMillis - lastForwardMillis);
#endif
          //checkLed_p(FORWARD);
          ec = checkIR();
          if ((currentMillis - lastDirMillis[FORWARD] >= dl) || fubar || ec != -1)
          {
            lastForwardMillis = currentMillis;
            bFlag = true;
          }
        }
        break;
      ////////////////////
      //  FORWARD_LEFT  //
      ////////////////////
      case FORWARD_LEFT:
        pcf20.write(PCF_leds[FORWARD_LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD_LEFT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec != FORWARD_LEFT && ec != -1)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[FORWARD_LEFT] >= 1000)
            {
              lastDebugMillis[FORWARD_LEFT] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 50;
            bridge[MOTOR_R][I1] = 1;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = 0;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            //checkLed_p(FORWARD_LEFT);
          }
        }
        break;
      ////////////////////
      //      LEFT      //
      ////////////////////
      case LEFT:
        // TODO forward or backard? for now, forward
        pcf20.write(PCF_leds[LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != LEFT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec == STOP)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[LEFT] >= 1000)
            {
              lastDebugMillis[LEFT] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 50;
            bridge[MOTOR_R][I1] = 1;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = 0;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            //checkLed_p(LEFT);
          }
        }
        break;
      ////////////////////
      //  BACKWARD_LEFT //
      ////////////////////
      case BACKWARD_LEFT:
        pcf20.write(PCF_leds[BACKWARD_LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != BACKWARD_LEFT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec == STOP)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[BACKWARD_LEFT] >= 1000)
            {
              lastDebugMillis[BACKWARD_LEFT] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = 0;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX - 50;
            bridge[MOTOR_L][I1] = 1;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            //checkLed_p(BACKWARD_LEFT);
          }
        }
        break;
      ////////////////////
      //    BACKWARD    //
      ////////////////////
      case BACKWARD:
        // TODO need to pick left or right direction to rotate to face backwards
        // for now; just turn right while going backwards
        pcf20.write(PCF_leds[BACKWARD], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != BACKWARD)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec == STOP)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[BACKWARD] >= 1000)
            {
              lastDebugMillis[BACKWARD] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 40;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 1;
            eSpeed[MOTOR_L] = SPEED_MAX - 40;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 1;
            runMotor();
            //checkLed_p(BACKWARD);
          }
        }
        break;
      ////////////////////
      //  BACKWARD_LEFT //
      ////////////////////
      case BACKWARD_RIGHT:
        pcf20.write(PCF_leds[BACKWARD_RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != BACKWARD_RIGHT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec == STOP)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[BACKWARD_RIGHT] >= 1000)
            {
              lastDebugMillis[BACKWARD_RIGHT] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 50;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 1;
            eSpeed[MOTOR_L] = bridge[MOTOR_L][I1] = bridge[MOTOR_L][I2] = 0;
            runMotor();
            //checkLed_p(BACKWARD_RIGHT);
          }
        }
        break;
      ////////////////////
      //      RIGHT     //
      ////////////////////
      case RIGHT:
        // TODO forward or backard? for now, backward
        pcf20.write(PCF_leds[RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != RIGHT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec == STOP)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[RIGHT] >= 1000)
            {
              lastDebugMillis[RIGHT] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = bridge[MOTOR_R][I1] = bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX - 50;
            bridge[MOTOR_L][I1] = 1;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            //checkLed_p(RIGHT);
          }
        }
        break;
      ////////////////////
      //  FORWARD_RIGHT //
      ////////////////////
      case FORWARD_RIGHT:
        pcf20.write(PCF_leds[FORWARD_RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD_RIGHT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec != FORWARD_RIGHT && ec != -1)
            bFlag = true;
          else
          {
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            if (currentMillis - lastDebugMillis[FORWARD_RIGHT] >= 1000)
            {
              lastDebugMillis[FORWARD_RIGHT] = currentMillis;
            }
#endif
            eSpeed[MOTOR_R] = bridge[MOTOR_R][I1] = bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX - 50;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 1;
            runMotor();
            //checkLed_p(FORWARD_RIGHT);
          }
        }
        break;
      case STOP:
        for (i = 0; i < STOP; i++)
          pcf20.write(PCF_leds[i], HIGH);
        if (dl != 1)
        {
          while (!bFlag)
          {
            currentMillis = millis();
            eSpeed[MOTOR_R] = bridge[MOTOR_R][I1] = bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = bridge[MOTOR_L][I1] = bridge[MOTOR_L][I2] = 0;
            runMotor();
            if ((currentMillis - lastDirMillis[STOP] >= dl) || fubar)
            {
              lastDirMillis[STOP] = currentMillis;
              bFlag = true;
            }
          }
        }
        else
        {
          eSpeed[MOTOR_R] = bridge[MOTOR_R][I1] = bridge[MOTOR_R][I2] = 0;
          eSpeed[MOTOR_L] = bridge[MOTOR_L][I1] = bridge[MOTOR_L][I2] = 0;
          runMotor();
        }
        break;
    }
  }
}

void printDir(int d, boolean el)
{
  switch (d)
  {
    case FORWARD: if (!el) Serial.print(F("FORWARD")); else Serial.println(F("FORWARD")); break;
    case FORWARD_LEFT: if (!el) Serial.print(F("FORWARD_LEFT")); else Serial.println(F("FORWARD_LEFT")); break;
    case LEFT: if (!el) Serial.print(F("LEFT")); else Serial.println(F("LEFT")); break;
    case BACKWARD_LEFT: if (!el) Serial.print(F("BACKWARD_LEFT")); else Serial.println(F("BACKWARD_LEFT")); break;
    case BACKWARD: if (!el) Serial.print(F("BACKWARD")); else Serial.println(F("BACKWARD")); break;
    case BACKWARD_RIGHT: if (!el) Serial.print(F("BACKWARD_RIGHT")); else Serial.println(F("BACKWARD_RIGHT")); break;
    case RIGHT: if (!el) Serial.print(F("RIGHT")); else Serial.println(F("RIGHT")); break;
    case FORWARD_RIGHT: if (!el) Serial.print(F("FORWARD_RIGHT")); else Serial.println(F("FORWARD_RIGHT")); break;
    case STOP: if (!el) Serial.print(F("STOP")); else Serial.println(F("STOP")); break;
  }
}

void runMotor()
{
  if (eSpeed[MOTOR_R])
    analogWrite(motorPins[MOTOR_R][EN], eSpeed[MOTOR_R]);
  else
    digitalWrite(motorPins[MOTOR_R][EN], LOW);
  digitalWrite(motorPins[MOTOR_R][I1], bridge[MOTOR_R][I1]);
  digitalWrite(motorPins[MOTOR_R][I2], bridge[MOTOR_R][I2]);

  if (eSpeed[MOTOR_L])
    analogWrite(motorPins[MOTOR_L][EN], eSpeed[MOTOR_L]);
  else
    digitalWrite(motorPins[MOTOR_L][EN], LOW);
  digitalWrite(motorPins[MOTOR_L][I1], bridge[MOTOR_L][I1]);
  digitalWrite(motorPins[MOTOR_L][I2], bridge[MOTOR_L][I2]);
}

void blink_p(int ledpin, unsigned long secs)
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillisBlink_array[ledpin] >= secs)
  {
    lastMillisBlink_array[ledpin] = currentMillis;
    pcf20.write(PCF_leds[ledpin], !pcf20.read(PCF_leds[ledpin]));
  }
}

void blink_d(int ledpin, unsigned long secs)
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillisBlink_d >= secs)
  {
    lastMillisBlink_d = currentMillis;
    digitalWrite(ledpin, !digitalRead(LED_BUILTIN));
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void checkLed_d(int ledpin)
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillisLedStop[ledpin] >= 30000)
  {
    lastMillisLedStop[ledpin] = currentMillis;
    if (digitalRead(ledpin))
      digitalWrite(ledpin, LOW);
  }
}

void checkLed_p(int ledpin)
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillisLedStop[ledpin] >= WAIT_DL)
  {
    lastMillisLedStop[ledpin] = currentMillis;
    /*
      Serial.print(F("led num: ")); Serial.print(PCF_leds[ledpin]); Serial.print(F(" [")); printDir(ledpin, false);
      Serial.print(F("] = ")); Serial.println(pcf20.read(PCF_leds[ledpin]));
      for (int i = 0; i < 8; i++)
      {
      printDir(i, false);
      Serial.print(F(": ")); Serial.println(pcf20.read(PCF_leds[i]));
      }
    */
    if (!pcf20.read(PCF_leds[ledpin]))
    {
      pcf20.write(PCF_leds[ledpin], HIGH);
      //digitalWrite(LED_BUILTIN, HIGH);
      //fubar = true;
    }
  }
}

void Snafu()
{
  moveRobot(STOP, 1);
  blink_d(internal_dead_bat_led, 500);
  blink_d(internal_low_bat_led, 250);
  blink_d(external_dead_bat_led, 500);
  blink_d(external_low_bat_led, 250);
  blink_p(FORWARD, 1000);
}

/* SERIAL COMM */
int getSerialData()
{
  int i = 0;
  int ret = -1;

  if (Serial.available())
  {
    // -=-=-=-=-=-=-=-= DELAY =-=-=-=-=-=-=-
    delay(100); //allows all serial sent to be received together
    // -=-=-=-=-=-=-=-= DELAY =-=-=-=-=-=-=-
    while (Serial.available() && i < 4)
      str[i++] = Serial.read();
    str[i++] = '\0';
  }
  if (i > 0)
    ret = atoi(str);
  return ret;
}

// BEGIN BATTERY
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
      digitalWrite(internal_low_bat_led, !(Percent < 50));
      digitalWrite(internal_dead_bat_led, !(Percent < 20));
    }
    else if (digitalRead(internal_low_bat_led) == HIGH)
      digitalWrite(internal_low_bat_led, LOW);
  }
  // external battery
  if (extBatAvail)
  {
    float curVolt = getVolt();
    digitalWrite(external_low_bat_led, !(curVolt < 5.0 && curVolt > 4));//((millis() % 3000 == 0) && getVolt() < 4));
    digitalWrite(external_dead_bat_led, !(curVolt < 4));//((millis() % 3000 == 0) && getVolt() < 4));
  }
  else // no battery was found
  {
    // every 10 seconds, test to see if the external battery is now present or not; resets extBatAvail flag
    if (currentMillis - lastMillisCheckBatt >= 10000)
    {
      lastMillisCheckBatt = currentMillis;
      getVolt();
    }
  }
}

float getVolt()
{
  float vin = 0.0;
  float vout = 0.0;
  float R1 = 980000.0;
  float R2 = 100000.0;
  int value;

  value = analogRead(voltPin);
  vout = (value * 5.04) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  if (vin >= 7.20)
    vin -= 7.20;
  else if (vin < 0.09)
    vin = 0.0;
  // test for external battery
  extBatAvail = (vin >= 5.0);
  return vin;
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

int checkIR()
{
  int dir = -1;

  if (irrecv.decode(&results))
  {
    //Serial.println(results.value, HEX);
    //digitalWrite(STATUS_PIN, HIGH);
    if (results.value == 0xFFA25D || results.value == 0xFF629D || results.value == 0xFFE21D ||
        results.value == 0xFF22DD || results.value == 0xFF02FD || results.value == 0xFFC23D ||
        results.value == 0xFFE01F || results.value == 0xFFA857 || results.value == 0xFF906F ||
        results.value == 0xFF6897 || results.value == 0xFF9867 || results.value == 0xFFB04F ||
        results.value == 0xFF30CF || results.value == 0xFF18E7 || results.value == 0xFF7A85 ||
        results.value == 0xFF10EF || results.value == 0xFF38C7 || results.value == 0xFF5AA5 ||
        results.value == 0xFF42BD || results.value == 0xFF4AB5 || results.value == 0xFF52AD)
    {
      dir = storeCode(&results);
    }
    irrecv.resume(); // resume receiver
    //digitalWrite(STATUS_PIN, LOW);
  }
  return dir;
}

int storeCode(decode_results *results)
{
  int r = -1;

  switch (results->value)
  {
    case 0xFFA25D: r = STOP; break; // CH -
    case 0xFF629D: r = STOP; break; // CH
    case 0xFFE21D: r = STOP; break; // CH +
    case 0xFF22DD: r = STOP; break; // |<<
    case 0xFF02FD: r = STOP; break; // >>|
    case 0xFFC23D: r = STOP; break; // |>||
    case 0xFFE01F: r = STOP; break; // VOL -
    case 0xFFA857: r = STOP; break; // VOL +
    case 0xFF906F: r = STOP; break; // EQ
    case 0xFF6897: r = STOP; break; // 0
    case 0xFF9867: r = STOP; break; // 100+
    case 0xFFB04F: r = STOP; break; // 200+
    case 0xFF30CF: r = FORWARD_LEFT; break;   // 1
    case 0xFF18E7: r = FORWARD; break;        // 2
    case 0xFF7A85: r = FORWARD_RIGHT; break;  // 3
    case 0xFF10EF: r = LEFT; break;           // 4
    case 0xFF38C7: r = STOP; break;           // 5
    case 0xFF5AA5: r = RIGHT; break;          // 6
    case 0xFF42BD: r = BACKWARD_LEFT; break;  // 7
    case 0xFF4AB5: r = BACKWARD; break;       // 8
    case 0xFF52AD: r = BACKWARD_RIGHT; break; // 9
    default:
      break;
  }
  return r;
}

void runDemo()
{
  demoMode = false;
  printDir(FORWARD, true); moveRobot(FORWARD, 7000);
  printDir(STOP, true); moveRobot(STOP, 1);
  printDir(FORWARD_LEFT, true); moveRobot(FORWARD_LEFT, 250);
  printDir(STOP, true); moveRobot(STOP, 1);
  printDir(RIGHT, true); moveRobot(RIGHT, 250);
  printDir(STOP, true); moveRobot(STOP, 1);
  printDir(BACKWARD_LEFT, true); moveRobot(BACKWARD_LEFT, 250);
  printDir(STOP, true); moveRobot(STOP, 1);
  printDir(BACKWARD, true); moveRobot(BACKWARD, 250);
  printDir(STOP, true); moveRobot(STOP, 1);
  printDir(BACKWARD_RIGHT, true); moveRobot(BACKWARD_RIGHT, 250);
}

