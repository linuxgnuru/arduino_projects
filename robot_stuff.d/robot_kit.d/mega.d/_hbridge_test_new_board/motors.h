#define S_L 0
#define S_R 1

#define M_L 0
#define M_R 1

typedef enum {
  EN,
  IN_A,
  IN_B
} pin_enum;

long lastCheckBattTime = millis();
int chCurrent[10];
boolean charging = false;
float Percent; // percentage of internal battery remaining
float myVin = 0.0;
