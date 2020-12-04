//We always have to include the library
#include "LedControl.h"

typedef enum {
  WRITE_MODE,
  ERASE_MODE
} w_e_enum;

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 2 (12) is connected to the DataIn
  pin 3 (11) is connected to the CLK
  pin 4 (10) is connected to LOAD
  We have only a single MAX72XX.
*/
LedControl lc = LedControl(4, 3, 2, 1);

// we always wait a bit between updates of the display
unsigned long delaytime = 100;
unsigned long pM_0 = 0;
unsigned long pM_1 = 0;

// debounce
const byte buttonPin = 7;
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

boolean write_erase = WRITE_MODE;
// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can't be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

const byte joyPinX = A2;
const byte joyPinY = A3;
const byte srandPin = A0;

void setup()
{
  randomSeed(analogRead(srandPin));
  pinMode(buttonPin, INPUT_PULLUP);
  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, 8);
  // and clear the display
  lc.clearDisplay(0);
  Serial.begin(9600);
}

int lastRanA = 9;
int lastRanB = 9;

unsigned long lastMillis = 0;

void loop()
{
  int ranA, ranB;
  boolean ok = false;
  byte data = 0;
  unsigned long curMillis = millis();

  if (curMillis - lastMillis >= 100)
  {
    lastMillis = curMillis;
    while (ok == false)
    {
      ranA = random(0, 8);
      if (ranA != lastRanA)
        ok = true;
    }
    Serial.print(ranA);
    Serial.print(F(" "));
    lastRanA = ranA;
    ok = false;
    while (ok == false)
    {
      ranB = random(0, 8);
      if (ranB != lastRanB)
        ok = true;
    }
    lastRanB = ranB;
    bitSet(data, ranB);
    Serial.print(ranB);
    Serial.print(F(" "));
    Serial.println(data, BIN);
    //lc.setRow(0, ranA, B00001000);
    lc.setRow(0, ranA, data);
    lc.setColumn(0, ranA, data);
  }
}

/*
  This function lights up a some Leds in a row.
  The pattern will be repeated on every row.
  The pattern will blink along with the row-number.
  row number 4 (index==3) will blink 4 times etc.
*/
void rows()
{
  for (int row = 0; row < 8; row++)
  {
    delay(delaytime);
    lc.setRow(0, row, B10100000);
    delay(delaytime);
    lc.setRow(0, row, (byte)0);
    for (int i = 0; i < row; i++)
    {
      delay(delaytime);
      lc.setRow(0, row, B10100000);
      delay(delaytime);
      lc.setRow(0, row, (byte)0);
    }
  }
}

/*
  This function lights up a some Leds in a column.
  The pattern will be repeated on every column.
  The pattern will blink along with the column-number.
  column number 4 (index==3) will blink 4 times etc.
*/
void columns()
{
  for (int col = 0; col < 8; col++)
  {
    delay(delaytime);
    lc.setColumn(0, col, B10100000);
    delay(delaytime);
    lc.setColumn(0, col, (byte)0);
    for (int i = 0; i < col; i++)
    {
      delay(delaytime);
      lc.setColumn(0, col, B10100000);
      delay(delaytime);
      lc.setColumn(0, col, (byte)0);
    }
  }
}

/*
  This function will light up every Led on the matrix.
  The led will blink along with the row-number.
  row number 4 (index==3) will blink 4 times etc.
*/
void single()
{
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      delay(delaytime);
      lc.setLed(0, row, col, true);
      delay(delaytime);
      for (int i = 0; i < col; i++)
      {
        lc.setLed(0, row, col, false);
        delay(delaytime);
        lc.setLed(0, row, col, true);
        delay(delaytime);
      }
    }
  }
}

void GridMark(int x, int y)
{
  lc.setLed(0, y, x, true);
}

void GridClear(int x, int y)
{
  lc.setLed(0, y, x, false);
}

