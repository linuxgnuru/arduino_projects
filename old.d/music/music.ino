
#include "pitches.h"

const int threshold = 10;    // minimum reading of the sensors that generates a note
int buttonPins[10];
int lastButtonState[10];
//int buttonState;

int Buzzer1 = 2;
int buttonState[10];         // variable for reading the pushbutton status

long lastDebounceTime[10];
long debounceDelay = 50;

// notes to play, corresponding to the 3 sensors:
int notes[] = {
  NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A4, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5
};

void setup()
{
  pinMode(Buzzer1, OUTPUT);
  // initialize the pushbutton pin as an input:
  for (int i = 0; i < 10; i++)
  {
    lastButtonState[i] = LOW;
    lastDebounceTime[i] = 0;
    buttonState[i] = 0;
    buttonPins[i] = i + 3;
    pinMode(buttonPins[i], INPUT);
  }
}

void loop()
{
  int reading;
  for (int i = 0; i < 10; i++)
  {
    reading = digitalRead(buttonPins[i]);
    if (reading != lastButtonState[i])
    {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay)
    {
      if (reading != buttonState[i])
        buttonState[i] = reading;
      if (buttonState[i] == HIGH)
      {
        tone(2, notes[i], 20);
      }
    }
    lastButtonState[i] = reading;
  }
#if 0
  for (int thisSensor = 0; thisSensor < 3; thisSensor++)
  {
    // get a sensor reading:
    int sensorReading = analogRead(thisSensor);
    // if the sensor is pressed hard enough:
    if (sensorReading > threshold)
    {
      // play the note corresponding to this sensor:
      tone(8, notes[thisSensor], 20);
    }
  }
#endif
}
