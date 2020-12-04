/*
  Adafruit Arduino - Lesson 15. Bi-directional Motor
*/

#define S_L 0
#define S_R 1

#define M_L 0
#define M_R 1

typedef enum {
  EN,
  IN_A,
  IN_B
} pin_enum;

int switchPins[] = {
  6, 7
};

//int potPin = A0;

const byte mPins[2][3] = {
  // EN  IN_A IN_B
  { 11, 2, 3 }, // motor 0 right
  { 10, 4, 5 }  // motor 1 left
};

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i < 2; i++)
  {
    pinMode(switchPins[i], INPUT_PULLUP);
    for (int j = 0; j < 3; j++)
      pinMode(mPins[i][j], OUTPUT);
  }
}

void loop()
{
  int speed = 255; //analogRead(potPin) / 4;
  boolean rev_l, rev_r;
  rev_r = digitalRead(switchPins[S_R]);
  rev_l = digitalRead(switchPins[S_L]);
  setMotor(M_L, speed, rev_l);
  setMotor(M_R, speed, rev_r);
}

void setMotor(int motor, int speed, boolean reverse)
{
  analogWrite(mPins[motor][EN], speed);
  digitalWrite(mPins[motor][IN_A], !reverse);
  digitalWrite(mPins[motor][IN_B], reverse);
}

