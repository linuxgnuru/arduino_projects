#define DEBUG
//#define DEBUG_

#define MAX_THRESHOLD 360

unsigned long lastMillis = 0;

char str[4];
int lowest = 1024;
int lowestP = 8;
int data[8];
int best;

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

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 8; i++)
  {
    data[i] = 1024;
  }
  Serial.println(F("-- start --"));
}

void loop()
{
  int ar;
//  int tmp;
  unsigned long currentMillis = millis();

  // only send data every quarter second
  if (currentMillis - lastMillis >= 250)
  {
    lastMillis = currentMillis;
    for (int i = A0; i < A7 + 1; i++)
    {
      ar = i - 14;
      data[ar] = analogRead(i);
#ifdef ANA_PRINT
      Serial.print(F("d["));
      Serial.print(ar);
      Serial.print(F("]: "));
      Serial.print(data[ar]);
      Serial.print(F("\t"));
#endif
    }
    for (int i = 0; i < 8; i++)
    {
      if (data[i] <= MAX_THRESHOLD && data[i] < lowest)
      {
        lowest = data[i];
        lowestP = i;
#ifdef DEBUG_
        Serial.print(F("lowestP: "));
        Serial.print(lowestP);
        Serial.print(F(" lowest: "));
        Serial.println(lowest);
#endif
      }
    }
#ifdef DEBUG_
    if (lowestP != 8)
    {
      Serial.print(F(" data["));
      Serial.print(lowestP);
      Serial.print(F("]: "));
      Serial.print(data[lowestP]);
      Serial.print(F(" lowest: "));
      Serial.println(lowest);
    }
#endif
    if (lowestP != 8 && lowest != 1024)
    {
      if (data[lowestP] <= MAX_THRESHOLD)
      {
        best = lowestP;
        //Serial.print(F("d[")); Serial.print(lowestP); Serial.print(F("] ")); Serial.println(data[lowestP]);
        //itoa(str, best, 10);
        printDir(best, true);
//        sprintf(str, "%d", best);
//        Serial.write(str, 4);
      }
    }
  }
  lowest = 1024;
  lowestP = 8;
}

void printDir(int d, boolean el)
{
  switch (d)
  {
    case -1: break;
    case FORWARD: if (!el) Serial.print(F("FORWARD")); else Serial.println(F("FORWARD")); break;
    case FORWARD_LEFT: if (!el) Serial.print(F("FORWARD_LEFT")); else Serial.println(F("FORWARD_LEFT")); break;
    case LEFT: if (!el) Serial.print(F("LEFT")); else Serial.println(F("LEFT")); break;
    case BACKWARD_LEFT: if (!el) Serial.print(F("BACKWARD_LEFT")); else Serial.println(F("BACKWARD_LEFT")); break;
    case BACKWARD: if (!el) Serial.print(F("BACKWARD")); else Serial.println(F("BACKWARD")); break;
    case BACKWARD_RIGHT: if (!el) Serial.print(F("BACKWARD_RIGHT")); else Serial.println(F("BACKWARD_RIGHT")); break;
    case RIGHT: if (!el) Serial.print(F("RIGHT")); else Serial.println(F("RIGHT")); break;
    case FORWARD_RIGHT: if (!el) Serial.print(F("FORWARD_RIGHT")); else Serial.println(F("FORWARD_RIGHT")); break;
    case STOP: if (!el) Serial.print(F("STOP")); else Serial.println(F("STOP")); break;
    default: Serial.print(F("ERROR ")); if (!el) Serial.print(d); else Serial.println(d); break;
  }
}

