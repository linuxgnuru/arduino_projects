#ifndef _HBRIDGE_L293D_H
#define _HBRIDGE_L293D_H

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class Hbridge_l293d
{
  private:
    struct motor {
      int id;
      byte pins[3];
      int curDirection;
      int curSpeed;
      int status;
    };

    typedef enum {
	  MOTOR_LEFT,
	  MOTOR_RIGHT
    } i_motor_enum;

    typedef enum {
      EN,
      IN1,
      IN2
    } pin_enum;

    boolean allStopped;
    motor motors[2];

    void WriteMotor(boolean motor_num, int speed, int in1, int in2);
    boolean checkSpeed(int speed);
    boolean ret;

public: 
    typedef enum {
      MOTOR_FORWARD,
      MOTOR_REVERSE,
      MOTOR_STOP
    } direction_enum;

    typedef enum {
      MOTOR0,
      MOTOR1
    } motor_enum;

explicit Hbridge_l293d(byte en1, byte in1, byte in2, byte en2, byte in3, byte in4);

explicit Hbridge_l293d();

void SetPins(boolean motor_num, byte en, byte in1, byte in2);
int GetDirection(boolean motor_num);
void SetDirection(boolean motor_num, int status);
boolean SetSpeed(boolean motor_num, int speed);
boolean SetSpeed(boolean motor_num, int speed, boolean reStart);
int GetSpeed(boolean motor_num);
void StartMotor(boolean motor_num);
boolean StartMotor(boolean motor_num, int speed);
void StopMotor(boolean motor_num);
void StopAllMotors();
boolean IsStopped();
/*
// no motor needed
boolean SetDir(boolean f_b);
int GetDir();
void SetCurMotor(boolean motor_num);
int GetCurMotor();
boolean SetSpeed(int speed);
int GetSpeed();
boolean StopMotor();
boolean StartMotor();
boolean StartMotor(int speed);
*/
};

#endif
