#include <PCF8574.h>
#include <NS_energyShield.h>
#include <IRremote.h>

#include "mpu_vars.h"
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

// function for mpu
void dmpDataReady()
{
  mpuInterrupt = true;
}

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
  // START MPU6050 STUFF
  for (int i = 0; i < 3; i++)
  {
    ypr[i] = 999;
    // h-bridge
    pinMode(motorPins[0][i], OUTPUT);
    pinMode(motorPins[1][i], OUTPUT);
  }
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  // initialize device
  mpu.initialize();
  // test connection
  if (mpu.testConnection() != true)
  {
    // error; can not connect to MPU6050
#ifdef _WIGG_DEBUG
    Serial.println(F("ERROR: can not connect to MPU6050"));
#endif
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(-64);
  mpu.setYGyroOffset(130);
  mpu.setZGyroOffset(-58);
  mpu.setXAccelOffset(-1529);
  mpu.setYAccelOffset(145);
  mpu.setZAccelOffset(1274);
  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    // turn on the DMP, now that it's ready
#ifdef _WIGG_DEBUG
    Serial.println(F("Enabling DMP..."));
#endif
    mpu.setDMPEnabled(true);
    // enable Arduino interrupt detection
#ifdef _WIGG_DEBUG
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
#endif
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
#ifdef _WIGG_DEBUG
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
#endif
    dmpReady = true;
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  else
  {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
#ifdef _WIGG_DEBUG
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
#endif
    digitalWrite(LED_BUILTIN, HIGH);
    emergStop = true;
  }
  // END MPU6050 STUFF
  getMPU();
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
      demoMode = false;
      printDir(FORWARD, true);
      moveRobot(FORWARD, 7000);

      printDir(STOP, true);
      moveRobot(STOP, 1);

      printDir(FORWARD_LEFT, true);
      moveRobot(FORWARD_LEFT, 250);

      printDir(STOP, true);
      moveRobot(STOP, 1);

      printDir(RIGHT, true);
      moveRobot(RIGHT, 250);

      printDir(STOP, true);
      moveRobot(STOP, 1);

      printDir(BACKWARD_LEFT, true);
      moveRobot(BACKWARD_LEFT, 250);

      printDir(STOP, true);
      moveRobot(STOP, 1);

      printDir(BACKWARD, true);
      moveRobot(BACKWARD, 250);

      printDir(STOP, true);
      moveRobot(STOP, 1);

      printDir(BACKWARD_RIGHT, true);
      moveRobot(BACKWARD_RIGHT, 250);
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
  boolean bFlag = false;
  int i;

  if (!fubar)
  {
    switch (dir)
    {
      case FORWARD:
        pcf20.write(PCF_leds[FORWARD], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          int ec = checkIR();
          if (ec == STOP)
            bFlag = true;
          else
          {

            eSpeed[MOTOR_R] = SPEED_MAX; // 0 - 255; might need some testing to see what valid values actually work
            //bridge[MOTOR_R][EN] = 1;
            bridge[MOTOR_R][I1] = 1;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX; // 0 - 255; might need some testing to see what valid values actually work
            //bridge[MOTOR_L][EN] = 1;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 1;
            runMotor();
            //getMPU();
            currentMillis = millis();
#ifdef _WIGG_DEBUG
            Serial.print(F("dl: ")); Serial.print(dl);
            Serial.print(F(" lastFM: ")); Serial.print(lastForwardMillis);
            Serial.print(F(" cur - lfm: ")); Serial.println(currentMillis - lastForwardMillis);
#endif
            //checkLed_p(FORWARD);
            if ((currentMillis - lastDirMillis[FORWARD] >= dl) || fubar)
            {
              lastForwardMillis = currentMillis;
              bFlag = true;
            }
          }
        }
        break;
      case FORWARD_LEFT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty - 42);
        ty_min = math_checker(ty - 47);
        pcf20.write(PCF_leds[FORWARD_LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD_LEFT)
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
            if (currentMillis - lastDebugMillis[FORWARD_LEFT] >= 1000)
            {
              lastDebugMillis[FORWARD_LEFT] = currentMillis;
              Serial.print(F("F_L ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 50;
            //bridge[MOTOR_R][EN] = 1;
            bridge[MOTOR_R][I1] = 1;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = 0;
            //bridge[MOTOR_L][EN] = 0;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            getMPU();
            //checkLed_p(FORWARD_LEFT);
            if ((yaw > ty_min && yaw < ty_max) || fubar) bFlag = true;
          }
        }
        break;
      case LEFT:
        // TODO forward or backard? for now, forward
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty - 87);
        ty_min = math_checker(ty - 93);
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
              Serial.print(F("  L ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 50;
            //bridge[MOTOR_R][EN] = 1;
            bridge[MOTOR_R][I1] = 1;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = 0;
            //bridge[MOTOR_L][EN] = 0;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            getMPU();
            //checkLed_p(LEFT);
            if ((yaw > ty_min && yaw < ty_max) || fubar) bFlag = true;
          }
        }
        break;
      case BACKWARD_LEFT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty - 42);
        ty_min = math_checker(ty - 47);
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
              Serial.print(F("B_L ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = 0;
            //bridge[MOTOR_R][EN] = 0;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX - 50;
            //bridge[MOTOR_L][EN] = 1;
            bridge[MOTOR_L][I1] = 1;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            getMPU();
            //checkLed_p(BACKWARD_LEFT);
            if ((yaw > ty_min && yaw < ty_max) || fubar) bFlag = true;
          }
        }
        break;
      case BACKWARD:
        // TODO need to pick left or right direction to rotate to face backwards
        // for now; just turn right while going backwards
        getMPU();
        ty = yaw;
        ty_max = -175; //math_checker(ty - 175);
        ty_min = 175; //math_checker(ty + 175);
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
              Serial.print(F("B   ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 40;
            //bridge[MOTOR_R][EN] = 1;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 1;
            eSpeed[MOTOR_L] = SPEED_MAX - 40;
            //bridge[MOTOR_L][EN] = 1;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 1;
            runMotor();
            getMPU();
            //checkLed_p(BACKWARD);
            if ((yaw > ty_min || yaw < ty_max) || fubar) bFlag = true;
          }
        }
        break;
      case BACKWARD_RIGHT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty + 47);
        ty_min = math_checker(ty + 42);
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
              Serial.print(F("B_R ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = SPEED_MAX - 50;
            //bridge[MOTOR_R][EN] = 1;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 1;
            eSpeed[MOTOR_L] = 0;
            //bridge[MOTOR_L][EN] = 0;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            getMPU();
            //checkLed_p(BACKWARD_RIGHT);
            if ((yaw > ty_min && yaw < ty_max) || fubar)            bFlag = true;
          }
        }
        break;
      case RIGHT:
        // TODO forward or backard? for now, backward
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty + 92);
        ty_min = math_checker(ty + 87);
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
              Serial.print(F("  R ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = 0;
            //bridge[MOTOR_R][EN] = 0;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX - 50;
            //bridge[MOTOR_L][EN] = 1;
            bridge[MOTOR_L][I1] = 1;
            bridge[MOTOR_L][I2] = 0;
            runMotor();
            getMPU();
            //checkLed_p(RIGHT);
            if ((yaw > ty_min && yaw < ty_max) || fubar)            bFlag = true;
          }
        }
        break;
      case FORWARD_RIGHT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty + 47);
        ty_min = math_checker(ty + 42);
        pcf20.write(PCF_leds[FORWARD_RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD_RIGHT)
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
            if (currentMillis - lastDebugMillis[FORWARD_RIGHT] >= 1000)
            {
              lastDebugMillis[FORWARD_RIGHT] = currentMillis;
              Serial.print(F("F_R ty:")); Serial.print(ty);
              Serial.print(F(" "));
              Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
            }
#endif
            eSpeed[MOTOR_R] = 0;
            //bridge[MOTOR_R][EN] = 0;
            bridge[MOTOR_R][I1] = 0;
            bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = SPEED_MAX - 50;
            //bridge[MOTOR_L][EN] = 1;
            bridge[MOTOR_L][I1] = 0;
            bridge[MOTOR_L][I2] = 1;
            runMotor();
            getMPU();
            //checkLed_p(FORWARD_RIGHT);
            if ((yaw > ty_min && yaw < ty_max) || fubar)            bFlag = true;
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
            eSpeed[MOTOR_R] = 0;
            //bridge[MOTOR_R][EN] = 0;
            bridge[MOTOR_R][I1] = bridge[MOTOR_R][I2] = 0;
            eSpeed[MOTOR_L] = 0;
            //bridge[MOTOR_L][EN] = 0;
            bridge[MOTOR_L][I1] = bridge[MOTOR_L][I2] = 0;
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
          eSpeed[MOTOR_R] = 0;
          //bridge[MOTOR_R][EN] = 0;
          bridge[MOTOR_R][I1] = bridge[MOTOR_R][I2] = 0;
          eSpeed[MOTOR_L] = 0;
          //bridge[MOTOR_L][EN] = 0;
          bridge[MOTOR_L][I1] = bridge[MOTOR_L][I2] = 0;
          runMotor();
        }
        break;
    }
  }
}

void getMPU()
{
  //unsigned long currentMillis = millis();
  // if programming failed, don't try to do anything
  if (!dmpReady)
    return;
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize)
  {
    // other program behavior stuff here
    // .
    // if you are really paranoid you can frequently test in between other
    // stuff to see if mpuInterrupt is true, and if so, "break;" from the
    // while() loop to immediately process the MPU data
    // .
  }
  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  // check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024)
  {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    //Serial.println(F("FIFO overflow!"));
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  }
  else if (mpuIntStatus & 0x02)
  {
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize)
      fifoCount = mpu.getFIFOCount();
    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;
    // display Euler angles in degrees
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    yaw = ypr[0] * 180 / M_PI;
    pitch = ypr[1] * 180 / M_PI;
    roll = ypr[2] * 180 / M_PI;
    //digitalWrite(13, !digitalRead(13));
    /*
      currentMillis = millis();
      if (currentMillis - lastDebugMillis[STOP] >= 1000)
      {
      lastDebugMillis[STOP] = currentMillis;
      Serial.print(F("[ yaw: "));
      Serial.print(yaw);
      Serial.println(F("]"));
      }
    */
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
  //digitalWrite(motorPins[MOTOR_R][EN], bridge[MOTOR_R][EN]);
  digitalWrite(motorPins[MOTOR_R][I1], bridge[MOTOR_R][I1]);
  digitalWrite(motorPins[MOTOR_R][I2], bridge[MOTOR_R][I2]);

  if (eSpeed[MOTOR_L])
    analogWrite(motorPins[MOTOR_L][EN], eSpeed[MOTOR_L]);
  else
    digitalWrite(motorPins[MOTOR_L][EN], LOW);
  //digitalWrite(motorPins[MOTOR_L][EN], bridge[MOTOR_L][EN]);
  digitalWrite(motorPins[MOTOR_L][I1], bridge[MOTOR_L][I1]);
  digitalWrite(motorPins[MOTOR_L][I2], bridge[MOTOR_L][I2]);
}

/*
  math_checker
  if (yaw > ty + 42 && yaw < ty + 47) // ty + 42 must not exceed +179
  if (yaw < ty - 42 && yaw > ty - 47) // ty - 42 must not exceed -179
  range -179 to 179
  +230 -->  +51
  -200 --> +158
   -70 -->  -70 (unchanged)

  example 1:
  r = WARD_LEFT;
  if (yaw > math_checker(ty + 42) && yaw < math_checker(ty + 47))

  example 2:
  r = WARD_LEFT;
  float in =  -70; float f = math_checker(in); // f == -70
  float in = -370; float f = math_checker(in); // f == -12
*/
float math_checker(float input)
{
  float r = input;

  if (input > 179 || input < -179)
  {
    if (input > 179)
      r -= 358; // 2 * 179
    else
      r += 358;
  }
  return r;
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
  /*
    if (results->value == 0xFFA25D) // CH -
    if (results->value == 0xFF629D) // CH
    if (results->value == 0xFFE21D) // CH +
    if (results->value == 0xFF22DD) // |<<
    if (results->value == 0xFF02FD) // >>|
    if (results->value == 0xFFC23D) // |>||
    if (results->value == 0xFFE01F) // VOL -
    if (results->value == 0xFFA857) // VOL +
    if (results->value == 0xFF906F) // EQ
    if (results->value == 0xFF6897) // 0
    if (results->value == 0xFF9867) // 100+
    if (results->value == 0xFFB04F) // 200+
    if (results->value == 0xFF30CF) // 1
    if (results->value == 0xFF18E7) // 2
    if (results->value == 0xFF7A85) // 3
    if (results->value == 0xFF10EF) // 4
    if (results->value == 0xFF38C7) // 5
    if (results->value == 0xFF5AA5) // 6
    if (results->value == 0xFF42BD) // 7
    if (results->value == 0xFF4AB5) // 8
    if (results->value == 0xFF52AD) // 9
  */
  switch (results->value)
  {
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

