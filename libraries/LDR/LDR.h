#ifndef LDR_H
#define LDR_H

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#ifndef MAX_THRESHOLD
#define MAX_THRESHOLD 360
#endif

class LDR
{
  private:
    unsigned long lastMillis = 0;
    char str[4];
    int lowest;
    int lowestP;
    int ar;
    int data[8];
    int best;
    const byte ledPins[8] = {
        2, 9, 8, 7, 6, 5, 4, 3
    };
    const byte checkPin = 10;
    boolean use_led = false;

  public:
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
    explicit LDR(boolean useLed);
    int getDir();
    void ledDemo();
    void printDir();
};
#endif
