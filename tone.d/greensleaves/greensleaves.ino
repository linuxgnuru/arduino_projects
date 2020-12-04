int piezo = 12;
int octave = 3;

char notes[] = {
  'C', 'd', 'D', 'e', 'E', 'F', 'g', 'G', 'a', 'A', 'b', 'B'
};

float freqs[] = {
  16.352, 17.324, 18.354, 19.445, 20.602, 21.827, 23.125, 24.500, 25.957, 27.500, 29.135, 30.868
};

char music[] = {
  'E', 'G', 'A', 'B', '+', 'd', '-', 'B', 'A', 'F', 'D', 'E', 'F', 'G', 'E', 'E', 'e', 'E', 'F', 'e', 'B', 'E', 'G', 'A', 'B', '+', 'd', '-', 'B', 'A', 'F', 'D', 'E', 'F', 'G', 'F', 'E', 'e', 'f', 'F', 'E', 'E', 'E', '+', 'D',
  'D', 'd', '-', 'B', 'A', 'F', 'D', 'E', 'F', 'G', 'E', 'E', 'e', 'E', 'F', 'e', 'B', '+', 'D', 'D', 'd', '-', 'B', 'A', 'F', 'D', 'E', 'F', 'G', 'F', 'E',
  'e', 'f', 'F', 'E', 'E', 'E', 'E'
};

byte beats[] = {
  2, 4, 2, 3, 1, 2, 4, 2, 3, 1, 2, 4, 2, 3, 1, 2, 4, 2, 4, 2, 4, 2, 3, 1, 2, 4, 2, 2, 1, 2, 3, 1, 2, 3, 2, 2, 4, 2, 6, 6, 3, 1, 2, 4, 2, 3, 1, 2, 4, 2, 3, 1, 2, 4, 2, 6, 6, 3, 1, 2, 4, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 6, 4, 2, 6
};

long thisNote = 0;
long thisBeat = 0;
int skips = 0;

void setup()
{
  pinMode(piezo, OUTPUT);
}

void loop()
{
  octave = 4;
  skips = 0;
  for (int i = 0; i < 83; i++)
  {
    //check if octave is changed
    if (music[i] == '+')
    {
      octave = octave + 1;
      skips++;
      continue;
    }
    if (music[i] == '-')
    {
      octave = octave - 1;
      skips++;
      continue;
    }
    //otherwise find the note;
    for (int k = 0; k < 12; k++)
    {
      if (music[i] == notes[k])
      {
        thisNote = (long)freqs[k] * pow(2, octave);
        break;
      }
    }
    thisBeat = (long)beats[i - skips];
    thisBeat = thisBeat * 200;
    piezoTone(thisNote, thisBeat);
  } //end loop through notes
  delay(1000);
}

void piezoTone(long freq, long duration)
{
  long aSecond = 1000000;
  long period = aSecond / freq;

  duration = duration * 1000;
  duration = duration / period;
  for (long k = 0; k < duration; k++)
  {
    digitalWrite(piezo, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(piezo, LOW);
    delayMicroseconds(period / 2);
  }
}

