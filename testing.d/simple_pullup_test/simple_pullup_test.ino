#define DEBUG

#include <DebugUtils.h>
unsigned int lM = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned int cM = millis();
  if (cM - lM >= 1000)
  {
    lM = cM;
    DEBUG_PRINTF("6", digitalRead(6));
    DEBUG_PRINTFLN(" 7", digitalRead(7));
  }
}
