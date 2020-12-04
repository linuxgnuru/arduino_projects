#define MOTOR_R 0
#define MOTOR_L 1

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

// boolean flags
boolean emergStop = false;
boolean stopMe = false;

