//We always have to include the library
#include "LedControl.h"

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 2 (12) is connected to the DataIn
  pin 3 (11) is connected to the CLK
  pin 4 (10) is connected to LOAD
  We have only a single MAX72XX.
*/
LedControl lc = LedControl(2, 3, 4, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;
unsigned long pM_0 = 0;
unsigned long pM_1 = 0;

void setup()
{
  pinMode(7, INPUT_PULLUP);
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

  //  writeArduinoOnMatrix();
  //  rows();
  //  columns();
  //  single();
  y = map(analogRead(A2), 1023, 0, 7, 0);
  x = map(analogRead(A3), 0, 1023, 7, 0);
  if (cM - pM_1 >= 50)
  {
    pM_1 = cM;
    GridMark(x, y);
  }
  if (digitalRead(7) == LOW)
    GridClear(8, 8);
  /*
  if (cM - pM_0 >= 50)
  {
    pM_0 = cM;
    GridClear(x, y);
  }*/
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
  if (x == 8 || y == 8)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        lc.setLed(0, i, j, false);
      }
    }
  }
  else
    lc.setLed(0, y, x, false);
}

