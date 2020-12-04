// millisecond counters
unsigned long lastMillisBlink[12] = {
  0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

// when should LEDs stop being on
unsigned long lastMillisLedStop[12] = {
  0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0
};

unsigned long lastDirMillis[9];
unsigned long lastForwardMillis = 0;

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

unsigned long lastDebugMillis[8];


