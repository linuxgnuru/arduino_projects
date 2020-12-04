#include <Keypad.h>

// TODO make the following also in it's own class
const byte ROWS = 4;
const byte COLS = 4;

// TODO move the pins around so that you have 3 that are PWM (R, G, B)
byte rowPins[ROWS] = { 2, 3, 4, 5 }; //connect to row pinouts
byte colPins[COLS] = { 6, 7, 8, 9 }; //connect to column pinouts

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

int curDigit = 0;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    /*
       get first 3 chars
       make sure it's 0 - 255 (use map or constrain)
       analogWrite R G or B the 3 digits

       Use A for red, B for green C for blue
       so, A 130 B 0 C 255 for color.
    */
    if (key == '*' || key == '#')
    {
    }
    else
    {
      if (key == 'A') // red color
      {

      }
    }
  }
}

int digit(char c)
{
  do
  {
    switch (key)
    {
    }
  } while (curDigit < 3);
}

