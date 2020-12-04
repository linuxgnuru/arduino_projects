#include <PCF8574.h>

#include "vars.h"
#include "mpu_vars.h"
#include "last_millis.h"

#define SPEED_MAX 255
#define SPEED_MIN 190
#define WAIT_DL 20000

//#define _DEBUG__

PCF8574 pcf20(0x20);

const byte PCF_leds[] = {
  //7, 1, 0, 2, 3, 4, 5, 6 // clockwise
  7, 6, 5, 4, 3, 2, 0, 1 // counter clockwise
};

//  I1 E  I2
const byte motorPins[2][3] = {
  { 3, 5, 4 }, // motor 0 right
  { 7, 6, 8 }  // motor 1 left
};

//  I1 E  I2
boolean bridge[2][3] = {
  { 0, 0, 0 }, // motor 0 right
  { 0, 0, 0 }  // motor 1 left
};

// speed = 0 - 255
int eSpeed[] = {
  0, // motor 0 right
  0  // motor 1 left
};

// function for mpu
void dmpDataReady()
{
  mpuInterrupt = true;
}

float yaw, pitch, roll;
float ty, ty_max, ty_min;
boolean demoMode = true;
boolean finalStop = false;
boolean oneTime = true;
int cur_dir = FORWARD;
int ctr = 0;
boolean fubar = false;

void setup()
{
  for (int i = 9; i < 13; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  for (int i = FORWARD; i < STOP; i++)
    lastDirMillis[i] = lastDebugMillis[i] = 0;
  Serial.begin(115200);
  Serial.println(F("--- START ---"));
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
    Serial.println(F("ERROR: can not connect to MPU6050"));
    digitalWrite(LED_BUILTIN, HIGH);
  }
  // load and configure the DMP
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    // enable Arduino interrupt detection
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
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
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
    digitalWrite(LED_BUILTIN, HIGH);
    emergStop = true;
  }
  // END MPU6050 STUFF
  getMPU();
  pcf20.begin();
  //#ifdef _DEBUG__
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
  //#endif
}

boolean donef = false;

void loop()
{
  //unsigned long currentMillis = millis();
  //int dir = FORWARD;

  if (demoMode)
  {
#ifdef _DEBUG__
    Serial.println(F("-=-=-= DEMO =-=-=-"));
#endif
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
#ifdef _DEBUG__
      if (oneTime)
      {
        Serial.println(F("=== dir ==="));
        oneTime = false;
      }
#endif
      if (cur_dir >= STOP)
      {
        //Serial.print(F("ctr: ")); Serial.println(ctr);
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
      blink_(FORWARD_LEFT, 1000, false);
      blink_(FORWARD_RIGHT, 500, false);
      blink_(BACKWARD_LEFT, 1000, false);
      blink_(BACKWARD_RIGHT, 500, false);
    }
    if (fubar && !donef)
    {
      blink_(LEFT, 500, true);
      blink_(RIGHT, 500, true);
    }
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
#ifdef _DEBUG__
        Serial.println(F("- FORWARD -"));
#endif
        //getMPU();
        pcf20.write(PCF_leds[FORWARD], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          /*
            if (currentMillis - lastDebugMillis[FORWARD] >= 1000)
            {
            lastDebugMillis[FORWARD] = currentMillis;
            Serial.print(F("F   yaw:")); Serial.println(yaw);
            }
          */
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
#ifdef __DEBUG__
          Serial.print(F("dl: "));
          Serial.print(dl);
          Serial.print(F(" lastFM: "));
          Serial.print(lastForwardMillis);
          Serial.print(F(" cur - lfm: "));
          Serial.println(currentMillis - lastForwardMillis);
#endif
          checkLed_p(FORWARD);
          if ((currentMillis - lastDirMillis[FORWARD] >= dl) || fubar)
          {
            lastForwardMillis = currentMillis;
            bFlag = true;
          }
        }
        break;
      case FORWARD_LEFT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty - 42);
        ty_min = math_checker(ty - 47);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[FORWARD_LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD_LEFT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
          if (currentMillis - lastDebugMillis[FORWARD_LEFT] >= 1000)
          {
            lastDebugMillis[FORWARD_LEFT] = currentMillis;
            Serial.print(F("F_L ty:")); Serial.print(ty);
            Serial.print(F(" "));
            Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
          }
#endif
          eSpeed[MOTOR_R] = SPEED_MIN;
          //bridge[MOTOR_R][EN] = 1;
          bridge[MOTOR_R][I1] = 1;
          bridge[MOTOR_R][I2] = 0;
          eSpeed[MOTOR_L] = 0;
          //bridge[MOTOR_L][EN] = 0;
          bridge[MOTOR_L][I1] = 0;
          bridge[MOTOR_L][I2] = 0;
          runMotor();
          getMPU();
          checkLed_p(FORWARD_LEFT);
          if ((yaw > ty_min && yaw < ty_max) || fubar)
            bFlag = true;
        }
        break;
      case LEFT:
        // TODO forward or backard? for now, forward
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty - 87);
        ty_min = math_checker(ty - 93);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != LEFT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
          if (currentMillis - lastDebugMillis[LEFT] >= 1000)
          {
            lastDebugMillis[LEFT] = currentMillis;
            Serial.print(F("  L ty:")); Serial.print(ty);
            Serial.print(F(" "));
            Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
          }
#endif
          eSpeed[MOTOR_R] = SPEED_MIN;
          //bridge[MOTOR_R][EN] = 1;
          bridge[MOTOR_R][I1] = 1;
          bridge[MOTOR_R][I2] = 0;
          eSpeed[MOTOR_L] = 0;
          //bridge[MOTOR_L][EN] = 0;
          bridge[MOTOR_L][I1] = 0;
          bridge[MOTOR_L][I2] = 0;
          runMotor();
          getMPU();
          checkLed_p(LEFT);
          if ((yaw > ty_min && yaw < ty_max) || fubar)
            bFlag = true;
        }
        break;
      case BACKWARD_LEFT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty - 42);
        ty_min = math_checker(ty - 47);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[BACKWARD_LEFT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != BACKWARD_LEFT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
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
          eSpeed[MOTOR_L] = SPEED_MIN;
          //bridge[MOTOR_L][EN] = 1;
          bridge[MOTOR_L][I1] = 1;
          bridge[MOTOR_L][I2] = 0;
          runMotor();
          getMPU();
          checkLed_p(BACKWARD_LEFT);
          if ((yaw > ty_min && yaw < ty_max) || fubar)
            bFlag = true;
        }
        break;
      case BACKWARD:
        // TODO need to pick left or right direction to rotate to face backwards
        // for now; just turn right while going backwards
        getMPU();
        ty = yaw;
        ty_max = -175; //math_checker(ty - 175);
        ty_min = 175; //math_checker(ty + 175);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[BACKWARD], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != BACKWARD)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
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
          bridge[MOTOR_L][I1] = 1;
          bridge[MOTOR_L][I2] = 0;
          runMotor();
          getMPU();
          //checkLed_p(BACKWARD);
          if ((yaw > ty_min || yaw < ty_max) || fubar)
            bFlag = true;
        }
        break;
      case BACKWARD_RIGHT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty + 47);
        ty_min = math_checker(ty + 42);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[BACKWARD_RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != BACKWARD_RIGHT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
          if (currentMillis - lastDebugMillis[BACKWARD_RIGHT] >= 1000)
          {
            lastDebugMillis[BACKWARD_RIGHT] = currentMillis;
            Serial.print(F("B_R ty:")); Serial.print(ty);
            Serial.print(F(" "));
            Serial.print(yaw); Serial.print(F(" > ")); Serial.print(ty_min); Serial.print(F(" && ")); Serial.print(yaw); Serial.print(F(" < ")); Serial.println(ty_max);
          }
#endif
          eSpeed[MOTOR_R] = SPEED_MIN;
          //bridge[MOTOR_R][EN] = 1;
          bridge[MOTOR_R][I1] = 0;
          bridge[MOTOR_R][I2] = 1;
          eSpeed[MOTOR_L] = 0;
          //bridge[MOTOR_L][EN] = 0;
          bridge[MOTOR_L][I1] = 0;
          bridge[MOTOR_L][I2] = 0;
          runMotor();
          getMPU();
          checkLed_p(BACKWARD_RIGHT);
          if ((yaw > ty_min && yaw < ty_max) || fubar)
            bFlag = true;
        }
        break;
      case RIGHT:
        // TODO forward or backard? for now, backward
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty + 92);
        ty_min = math_checker(ty + 87);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != RIGHT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
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
          eSpeed[MOTOR_L] = SPEED_MIN;
          //bridge[MOTOR_L][EN] = 1;
          bridge[MOTOR_L][I1] = 1;
          bridge[MOTOR_L][I2] = 0;
          runMotor();
          getMPU();
          checkLed_p(RIGHT);
          if ((yaw > ty_min && yaw < ty_max) || fubar)
            bFlag = true;
        }
        break;
      case FORWARD_RIGHT:
        getMPU();
        ty = yaw;
        ty_max = math_checker(ty + 47);
        ty_min = math_checker(ty + 42);
#ifdef _DEBUG__
        Serial.print(F("max: ")); Serial.println(ty_max);
        Serial.print(F("min: ")); Serial.println(ty_min);
#endif
        pcf20.write(PCF_leds[FORWARD_RIGHT], LOW);
        for (i = 0; i < STOP; i++)
        {
          if (i != FORWARD_RIGHT)
            pcf20.write(PCF_leds[i], HIGH);
        }
        while (!bFlag)
        {
          currentMillis = millis();
#ifdef _DEBUG__
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
          eSpeed[MOTOR_L] = SPEED_MIN;
          //bridge[MOTOR_L][EN] = 1;
          bridge[MOTOR_L][I1] = 0;
          bridge[MOTOR_L][I2] = 1;
          runMotor();
          getMPU();
          checkLed_p(FORWARD_RIGHT);
          if ((yaw > ty_min && yaw < ty_max) || fubar)
            bFlag = true;
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
  if (yaw > math_checker(ty + 42) && yaw < math_checker(ty + 47))

  example 2:
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

void blink_(int ledP, unsigned long secs, boolean dig)
{
  unsigned long currentMillis = millis();

  if (currentMillis - lastMillisBlink[ledP] >= secs)
  {
    lastMillisBlink[ledP] = currentMillis;
    pcf20.write(PCF_leds[ledP], !pcf20.read(PCF_leds[ledP]));
    if (dig)
    {
      digitalWrite(9, !digitalRead(LED_BUILTIN));
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
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
    Serial.print(F("checkLed ["));
    printDir(ledpin, false);
    Serial.print(F("] = "));
    Serial.println(pcf20.read(PCF_leds[ledpin]));
    if (pcf20.read(ledpin))
    {
      pcf20.write(PCF_leds[ledpin], HIGH);
      //digitalWrite(LED_BUILTIN, HIGH);
      fubar = true;
    }
  }
}

