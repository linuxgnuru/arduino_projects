/*
 *
 */

#include "Hbridge_l293d.h"

Hbridge_l293d::Hbridge_l293d(byte en1, byte in1, byte in2, byte en2, byte in3, byte in4)
{
  allStopped = false;
  motors[MOTOR0].id = 0;
  motors[MOTOR1].id = 1;

  motors[MOTOR0].status = MOTOR_STOP;
  motors[MOTOR1].status = MOTOR_STOP;

  motors[MOTOR0].curDirection = MOTOR_FORWARD;
  motors[MOTOR1].curDirection = MOTOR_FORWARD;

  motors[MOTOR0].curSpeed = -1;
  motors[MOTOR1].curSpeed = -1;

  SetPins(MOTOR0, en1, in1, in2);
  SetPins(MOTOR1, en2, in3, in4);
}

Hbridge_l293d::Hbridge_l293d()
{
  allStopped = false;
  motors[MOTOR0].id = 0;
  motors[MOTOR1].id = 1;

  motors[MOTOR0].status = MOTOR_STOP;
  motors[MOTOR1].status = MOTOR_STOP;

  motors[MOTOR0].curDirection = MOTOR_FORWARD;
  motors[MOTOR1].curDirection = MOTOR_FORWARD;

  motors[MOTOR0].curSpeed = -1;
  motors[MOTOR1].curSpeed = -1;

  SetPins(MOTOR0, 5, 8, 2);
  SetPins(MOTOR1, 6, 3, 7);
}

// if (!mybridge.setPins(1, en, in1, in2)) printf("error");
// TODO after pins are set, then do something?
void Hbridge_l293d::SetPins(boolean motor_num, byte en, byte in1, byte in2)
{
  motors[motor_num].pins[EN] = en;
  motors[motor_num].pins[IN1] = in1;
  motors[motor_num].pins[IN2] = in2;
  pinMode(motors[motor_num].pins[EN], OUTPUT);
  pinMode(motors[motor_num].pins[IN1], OUTPUT);
  pinMode(motors[motor_num].pins[IN2], OUTPUT);
}

int Hbridge_l293d::GetDirection(boolean motor_num)
{
  return motors[motor_num].curDirection;
}

void Hbridge_l293d::SetDirection(boolean motor_num, int status)
{
  motors[motor_num].status = status;
  // this allows you to stop the motor
  if (status == MOTOR_STOP)
  {
    StopMotor(motor_num);
    // FIXME should i add the following? just to make sure it's set to something?
    //motors[motor_num].curDirection = MOTOR_FORWARD;
  }
  else
    motors[motor_num].curDirection = status;
}

void Hbridge_l293d::WriteMotor(boolean motor_num, int speed, int in1, int in2)
{
  analogWrite(motors[motor_num].pins[EN], speed);
  digitalWrite(motors[motor_num].pins[IN1], in1);
  digitalWrite(motors[motor_num].pins[IN2], in2);
}

boolean Hbridge_l293d::checkSpeed(int speed)
{
  return (speed <= 255 && speed >= 150);
}

// TODO create an error class?
boolean Hbridge_l293d::SetSpeed(boolean motor_num, int speed)
{ 
  ret = true;
  // set a default speed
  if (speed == -1)
    motors[motor_num].curSpeed = 180;
  else
  {
    if (checkSpeed(speed))
      motors[motor_num].curSpeed = speed;
    else
      ret = false;
  }
  return ret;
}

boolean Hbridge_l293d::SetSpeed(boolean motor_num, int speed, boolean reStart)
{ 
  ret = true;
  if (checkSpeed(speed))
  {
    motors[motor_num].curSpeed = speed;
    if (reStart)
    {
      StopMotor(motor_num);
      WriteMotor(motor_num, motors[motor_num].curSpeed, !motors[motor_num].curDirection, motors[motor_num].curDirection);
    }
  }
  else
    ret = false;
  return ret;
}

int Hbridge_l293d::GetSpeed(boolean motor_num)
{ 
  return motors[motor_num].curSpeed;
}

void Hbridge_l293d::StartMotor(boolean motor_num)
{
  SetSpeed(motor_num, motors[motor_num].curSpeed);
  motors[motor_num].status = motors[motor_num].curDirection;
  WriteMotor(motor_num, motors[motor_num].curSpeed, !motors[motor_num].curDirection, motors[motor_num].curDirection);
}

boolean Hbridge_l293d::StartMotor(boolean motor_num, int speed)
{
  ret = true;
  if (SetSpeed(motor_num, speed))
  {
    motors[motor_num].status = motors[motor_num].curDirection;
    WriteMotor(motor_num, motors[motor_num].curSpeed, !motors[motor_num].curDirection, motors[motor_num].curDirection);
  }
  else
    ret = false;
  return ret;
}

void Hbridge_l293d::StopMotor(boolean motor_num)
{
  WriteMotor(motor_num, 0, LOW, LOW);
}

void Hbridge_l293d::StopAllMotors()
{
  StopMotor(0);
  StopMotor(1);
  allStopped = true;
}

boolean Hbridge_l293d::IsStopped()
{
  return allStopped;
}
/*
// using curMotor; no motor_num given
void Hbridge_l293d::SetDir(boolean f_b)
{
  StopMotor(curMotor);
  curDir[curMotor] = f_b;
  StartMotor(curMotor);
}

boolean Hbridge_l293d::GetDir()
{
  return curDir[curMotor];
}

void Hbridge_l293d::SetCurMotor(boolean motor_num)
{
  curMotor = motor_num;
}

boolean Hbridge_l293d::GetCurMotor()
{
  return curMotor;
}

boolean Hbridge_l293d::SetSpeed(int speed)
{
  ret = true;
  if (speed <= 255 && speed >= 150)
  {
    StopMotor(curMotor);
    curSpeed[curMotor] = speed;
    StartMotor(curMotor);
  }
  else
    ret = false;
  return ret;
}

int Hbridge_l293d::GetSpeed()
{
  return (isMotor ? curSpeed[curMotor] : -1);
}

void Hbridge_l293d::StartMotor()
{
    // set a default speed if none exist
    if (curSpeed[curMotor] == -1)
      curSpeed[curMotor] = 180;
    SetSpeed(curSpeed);
    digitalWrite(pins[curMotor][IN1], !curDir[curMotor]); // IN1, true
    digitalWrite(pins[curMotor][IN2], curDir[curMotor]); // IN2, false
}

boolean Hbridge_l293d::StartMotor(int speed)
{
  ret = true;
  if (speed <= 255 && speed >= 150)
  {
    SetSpeed(speed);
    digitalWrite(pins[curMotor][IN1], !curDir[curMotor]); // IN1, true
    digitalWrite(pins[curMotor][IN2], curDir[curMotor]); // IN2, false
  }
  else
    ret = false;
  return ret;
}

void Hbridge_l293d::StopMotor()
{
  StopMotor(curMotor);
}

int Hbridge_l293d::GetMotorStatus()
{
  return (motorStatus[curMotor]);
}
*/
