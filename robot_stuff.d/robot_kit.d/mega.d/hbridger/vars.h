#define MOTOR_R 0
#define MOTOR_L 1

#define SONAR_NUM      3 // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

typedef enum {
  A_OK,
  F_NO_FLOOR,
  B_NO_FLOOR,
  WALL_BEHIND,
  S_LEFT,
  S_CENTER,
  S_RIGHT
} sensor_status;

// Maybe create a hbridge class to throw all this stuff in
typedef enum {
  EN,
  IN_A,
  IN_B
} pin_enum;

boolean reverse[2] = {
  false, // motor 0 right
  false  // motor 1 left
};

// The following was a test to
/*
  typedef enum {
  ALL_OK = 0,
  DOWN_F = 1,
  DOWN_B = 2,
  BACK_B = 4
  } status_enum;
*/

// Boolean flags

boolean emergStop = false;
boolean sens_hit_flag = false;
boolean stopMe = false;
boolean tooClose = false;
boolean stopBlinking = false;

boolean hasChanged_fd = false;
boolean hasChanged_bd = false;
boolean hasChanged_bu = false;

boolean extBatAvail = true;

// millisecond counters
unsigned int lastMillisSensor = 0;
unsigned int lastMillisStopped = 0;
unsigned int lastMillisBackup = 0;
unsigned int lastMillisBat = 0;
unsigned int lastMillisBlink = 0;
unsigned int lastMillisCheckBatt = 0;
unsigned int lastMillisBlinkKill = 0;

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

int problemState = A_OK;

int curSpeed = 0;
int curMotorNum = MOTOR_R;

long lastCheckBattTime = millis();
int chCurrent[10];
boolean charging = false;
float Percent; // percentage of internal battery remaining
float myVin = 0.0;

// --- IMPORTANT variables / flags ---
//boolean backup_back, backup_left, backup_right, backup_back_left;

