#include <pit.h>

#include "melody.h"

#define NO_SOUND 0

void setup()
{
  for (int Note = 0; Note < 54; Note++)
  {
    int duration = pace / noteDurations1[Note]; // adjust duration with the pace of music
    tone(8, melody1[Note], duration);
    delay(duration * 1.2);
  }
  for (int Note = 0; Note < 54; Note++)
  {
    int duration = pace / noteDurations2[Note]; // adjust duration with the pace of music
    tone(8, melody2[Note], duration);
    delay(duration * 1.2);
  }
}

void loop()
{
}

