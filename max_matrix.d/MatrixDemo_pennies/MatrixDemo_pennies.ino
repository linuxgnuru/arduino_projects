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
LedControl lc = LedControl(2, 3, 4, 1);

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

int lastX = 9;
int lastY = 9;

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);
  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, 8);
  // and clear the display
  lc.clearDisplay(0);
}

void loop()
{
  unsigned long cM = millis();
  // x and y have to be in the range of 0 - 7
  int x, y;

  //  rows();
  //  columns();
  //  single();
  /*

    y   +-----------------+
    y   |A       B       C|
    y   |                 |
    y   |                 |
    y   |D       E       F|
    y   |                 |
    y   |                 |
    y   |                 |
    y   |G       H       I|
    y   +-----------------+
       xxxxxxxxxxxxxxxxxxx

          7 6 5 4 3 2 1 0
         0               0
         1               1
         2               2
         3               3
         4               4
         5               5
         6               6
         7               7
          7 6 5 4 3 2 1 0

           X, Y
       A = 7, 0
       B = 4, 0 or 3, 0
       C = 0, 0
       D = 4, 7 or 3, 7
       E = 4, 4; 4, 3; 3, 3; 3, 4
       F = 0, 3 or 0, 4
       G = 7, 7
       H = 4, 7 or 3, 7
       I = 0, 7
  */
  x = map(analogRead(joyPinX), 1023, 0, 7, 0);
  y = map(analogRead(joyPinY), 0, 1023, 7, 0);
  if (cM - pM_1 >= 50)
  {
    pM_1 = cM;
    if ((x != lastX) || (y != lastY))
    {
      if (x != lastX)
        lastX = x;
      if (y != lastY)
        lastY = y;
      GridClear(lastY, lastX);
    }
    // either clear the whole display or toggle gridmark to grid erase
    int buttonReading = digitalRead(buttonPin);
    if (buttonReading != lastButtonState)
      lastDebounceTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      if (buttonReading != buttonState)
      {
        buttonState = buttonReading;
        if (buttonState == LOW)
          lc.clearDisplay(0);
        //write_erase = !write_erase;
      }
    }
    lastButtonState = buttonReading;
    //if (write_erase == WRITE_MODE)
    GridMark(y, x);
    /*else
      {
      //lc.clearDisplay(0);
      GridClear(y, x);
      }*/
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

