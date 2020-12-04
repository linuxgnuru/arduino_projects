//#ifndef __VARS_H
//#define __VARS_H

// for battery shield
unsigned long lastCheckBattTime = millis();
int chCurrent[10];
boolean charging = false;
float Percent; // percentage of internal battery remaining
float myVin = 0.0;

// enums
typedef enum {
  FORWARD,
  FORWARD_LEFT,
  LEFT,
  BACKWARD_LEFT,
  BACKWARD,
  BACKWARD_RIGHT,
  RIGHT,
  FORWARD_RIGHT,
  STOP
} dir_enum;

typedef enum {
  I1,
  EN,
  I2
} pin_enum;

typedef enum {
  MOTOR_R,
  MOTOR_L
} motor_enum;

// boolean flags
boolean extBatAvail = true;
boolean emergStop = false;
boolean stopMe = false;
boolean fubar = false;
boolean demoMode = true;
boolean finalStop = false;
boolean oneTime = true;

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

char str[4];

float yaw, pitch, roll;
float ty, ty_max, ty_min;
int cur_dir = FORWARD;
int ctr = 0;

// millisecond counters
unsigned long lastMillisBlink_array[12] = {
  0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

// when should LEDs stop being on
unsigned long lastMillisLedStop[12] = {
  0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

unsigned long lastDirMillis[9];
unsigned long lastDebugMillis[9];

unsigned long lastMillisBlink_d = 0;
unsigned long lastForwardMillis = 0;

unsigned long lastMillisCheckBatt = 0;
unsigned long lastMillisBat = 0;

decode_results results;

// Storage for the recorded code
int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state

/*
unsigned long lastForwardMillis = 0;
unsigned long lastForwardLeftMillis = 0;
unsigned long lastLeftMillis = 0;
unsigned long lastBackardLeftMillis = 0;
unsigned long lastBackardMillis = 0;
unsigned long lastBackardRightMillis = 0;
unsigned long lastRightMillis = 0;
unsigned long lastForwardRightMillis = 0;
*/
//#endif
