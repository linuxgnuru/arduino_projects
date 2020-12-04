#define MOTOR_RIGHT 0
#define MOTOR_LEFT 1

typedef enum {
  EN,
  IN_A,
  IN_B,
  SW
} pin_enum;

// to use this method:  mPins[MOTOR_RIGHT][EN]
const byte mPins[2][4] = {
// EN IN_A IN_B SW
  { 6, A3, A1, 8 },
  { 5, 4, 4, 10 }
};

#define SPEED 127

void setup()
{
  for (int i = 0; i < 2; i++)
  {
    pinMode(mPins[i][SW], INPUT);
    for (int j = 0; j < 3; j++)
      pinMode(mPins[i][j], OUTPUT);
  }
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
}
 
void loop()
{
  int speed = SPEED;
  boolean reverse = true;
  boolean o_r = false;

  for (int i = 0; i < 2; i++)
  {
    if (o_r != reverse)
      o_r = reverse;
    reverse = digitalRead(mPins[i][SW]);
    if (o_r != reverse)
      digitalWrite((i == 0 ? 12 : 7), !digitalRead((i == 0 ? 12 : 7)));
    setMotor(i, speed, reverse);
  }
}
 
void setMotor(int motorNum, int speed, boolean reverse)
{
  analogWrite(mPins[motorNum][EN], speed);
  digitalWrite(mPins[motorNum][IN_A], reverse);
  digitalWrite(mPins[motorNum][IN_B], !reverse);
}

