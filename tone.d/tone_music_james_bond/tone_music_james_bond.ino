#include "pit.h"

#define NO_SOUND 0

int melody[] = {
  /*
  NOTE_G4, NOTE_G4, NO_SOUND, NOTE_G4, NOTE_G4, NO_SOUND, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,
   NOTE_B3, NOTE_G3, NOTE_C4, NOTE_G3, NOTE_CS4, NOTE_G3, NOTE_C4, NOTE_G3, NOTE_B3, NOTE_G3, NOTE_C4,
   NOTE_G3, NOTE_CS4, NOTE_G3, NOTE_C4, NOTE_G3,
   NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
   NOTE_E4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_E4,
   */
  // introduction
  NOTE_G4, NOTE_G4, NOTE_G2,
  NOTE_G4, NOTE_G4, NOTE_G2,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_F3,
  NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_F3,
  NOTE_E4,  NOTE_F4,  NOTE_F4,  NOTE_F4,  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4,  NOTE_G4,  NOTE_G4,  NOTE_G4,  NOTE_G4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4,  NOTE_F4,  NOTE_F4,  NOTE_F4,  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4,  NOTE_G4,  NOTE_G4,  NOTE_G4,  NOTE_G4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_DS5, NOTE_D5,  NOTE_B4,  NOTE_A4,  NOTE_B4,
  NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_F3,
  NOTE_E3, NOTE_F3, NOTE_FS3, NOTE_F3,
  NOTE_E4,  NOTE_F4,  NOTE_F4,  NOTE_F4,  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4,  NOTE_G4,  NOTE_G4,  NOTE_G4,  NOTE_G4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4,  NOTE_F4,  NOTE_F4,  NOTE_F4,  NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4,  NOTE_G4,  NOTE_G4,  NOTE_G4,  NOTE_G4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_DS5, NOTE_D5,  NOTE_B4,  NOTE_A4,  NOTE_B4,
  NOTE_E4,  NOTE_G4,  NOTE_DS5, NOTE_D5,  NOTE_G4, NOTE_B4,
  NOTE_B4,  NOTE_FS5, NOTE_F5,  NOTE_B4,  NOTE_D5, NOTE_AS5,
  NOTE_A5,  NOTE_F5,  NOTE_A5,  NOTE_DS6, NOTE_D6, NO_SOUND
};

int noteDurations[] = {
  /*
  8,  8,  2, 8, 8, 2, 16, 8, 16, 8, 8,
   2,  4,  2, 4, 2, 4,  2, 4,  2, 4, 2, 4, 2, 4, 2, 4,
   8, 16, 16, 8, 4, 8,  8, 8,
   8, 16, 16, 8, 4, 8,  8, 8,
   */
  8, 8, 2,  8, 8, 2, 16, 8, 16, 8, 8,
  2, 2, 2, 2,
  2, 2, 2, 2,
  8, 16, 16, 8, 4, 8, 8, 8,
  8, 16, 16, 8, 4, 8, 8, 8,
  8, 16, 16, 8, 4, 8, 8, 8,
  8, 16, 16, 8, 4, 8, 8, 8,
  8,  2,  8, 8, 1,
  2, 2, 2, 2,
  2, 2, 2, 2,
  8, 16, 16, 8, 4, 8, 8, 8,
  8, 16, 16, 8, 4, 8, 8, 8,
  8, 16, 16, 8, 4, 8, 8, 8,
  8, 16, 16, 8, 4, 8, 8, 8,
  8,  2,  8, 8, 1,
  8,  4,  8, 4, 8, 8,
  8,  8,  4, 8, 4, 8,
  4,  8,  4, 8, 3
};

int pace = 1450; // change pace of music ("speedy")

void setup()
{
  for (int Note = 0; Note < 118; Note++)
  {
    int duration = pace / noteDurations[Note]; // adjust duration with the pace of music
    tone(8, melody[Note], duration);
    delay(duration * 1.2);
  }
}

void loop()
{
}

