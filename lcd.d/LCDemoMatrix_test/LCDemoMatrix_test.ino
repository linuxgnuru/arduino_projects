//We always have to include the library
#include "LedControl.h"
#include <string.h>

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(12, 11, 10, 1);

//const int vccPin = 
int buttonPin = 7;
int  buttonState; // current reading from the input pin
int  lastButtonState = LOW; // previous reading from the input pin
long lastDebounceTime = 0; // last time the output pin was toggled

long debounceDelay = 50; // debounce time; increase if the output flickers

// we always wait a bit between updates of the display
unsigned long delaytime = 500;

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

const byte alphabetArray[][8] = {
  { // A
    B11111000,
    B00010100,
    B00010010,
    B00010001,
    B00010001,
    B00010010,
    B00010100,
    B11111000
  }, { // B
    B11111111,
    B10001001,
    B10001001,
    B10001001,
    B10011011,
    B01110110,
    B01110100,
    B00000000
  }, { // C
    B01111110,
    B11000011,
    B10000001,
    B10000001,
    B10000001,
    B10000001,
    B11000011,
    B01000010
  }, { // D
    B11111111,
    B10000001,
    B10000001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000
  }, { // E
    B11111111,
    B10001001,
    B10001001,
    B10001001,
    B10001001,
    B10000001,
    B10000001,
    B00000000
  }, { // F
    B11111111,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00000001,
    B00000001,
    B00000000
  }, { // G
    B01111110,
    B11000011,
    B10000001,
    B10000001,
    B10010001,
    B11110001,
    B01110111,
    B00010010
  }, {
    B11111111,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B11111111
  }, {
    B00000000,
    B00000000,
    B10000001,
    B11111111,
    B10000001,
    B00000000,
    B00000000,
    B00000000
  }, {
    B00010000,
    B01110000,
    B10010000,
    B10000001,
    B01111111,
    B00000001,
    B00000000,
    B00000000
  }, {
    B11111111,
    B00011000,
    B00011000,
    B00100100,
    B01100110,
    B01000010,
    B10000001,
    B10000001
  }, {
    B11111111,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B00000000
  }, {
    B11111111,
    B00000010,
    B00000100,
    B00001000,
    B00001000,
    B00000100,
    B00000010,
    B11111111
  }, {
    B11111111,
    B00000010,
    B00000100,
    B00001000,
    B00010000,
    B00100000,
    B01000000,
    B11111111
  }, {
    B00111100,
    B01000010,
    B11000011,
    B10000001,
    B10000001,
    B11000011,
    B01000010,
    B00111100
  }, {
    B11111110,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00000110
  }, {
    B00111100,
    B01000010,
    B10000001,
    B10000001,
    B11000001,
    B01100001,
    B01110010,
    B10011100
  }, {
    B11111110,
    B00001001,
    B00011001,
    B00011001,
    B00101001,
    B01001001,
    B10001001,
    B00000110
  }, {
    B01000110,
    B10001001,
    B10001001,
    B10011001,
    B10011001,
    B10010001,
    B10010001,
    B01100010
  }, {
    B00000001,
    B00000001,
    B00000001,
    B11111111,
    B00000001,
    B00000001,
    B00000001,
    B00000000
  }, {
    B01111111,
    B11000000,
    B10000000,
    B10000000,
    B10000000,
    B10000000,
    B11000000,
    B01111111
  }, {
    B00001111,
    B00011000,
    B01100000,
    B10000000,
    B10000000,
    B01100000,
    B00011000,
    B00001111
  }, {
    B00011111,
    B11111000,
    B01000000,
    B00110000,
    B00110000,
    B01000000,
    B11111000,
    B00011111
  }, {
    B11000011,
    B01100110,
    B00111000,
    B00011000,
    B00011000,
    B00011100,
    B01100110,
    B11000011
  }, {
    B00000011,
    B00000110,
    B00001100,
    B11111000,
    B11111000,
    B00001100,
    B00000110,
    B00000011
  }, {
    B11000001,
    B11100001,
    B10110001,
    B10011001,
    B10011001,
    B10001101,
    B10000111,
    B10000011
  }, {
    B00111100,
    B01000010,
    B10010101,
    B10100001,
    B10100001,
    B10010101,
    B01000010,
    B00111100
  }, {
  }
  
};

void habari()
{
  byte h[8] = {
    B11111111,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B11111111
  }; // h
  byte a[8] = {
    B00011111,
    B00101000,
    B01001000,
    B10001000,
    B10001000,
    B01001000,
    B00101000,
    B00011111
  }; // a
  byte b[8] = {
    B00000000,
    B00101110,
    B01101110,
    B11011001,
    B10010001,
    B10010001,
    B10010001,
    B11111111
  }; // b
  byte r[8] = {
    B01100000,
    B10010001,
    B10010010,
    B10010100,
    B10011000,
    B10011000,
    B10010000,
    B01111111
  }; // i
  byte i[8] = {
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B11111111,
    B10000001,
    B00000000,
    B00000000
  };

  // now display them one by one with a small delay
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, h[zz]);
  pause();
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, a[zz]);
  pause();
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, b[zz]);
  pause();
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, a[zz]);
  pause();
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, r[zz]);
  pause();
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, i[zz]);
  pause();
  for (int zz = 0; zz < 8; zz++)
    lc.setRow(0, zz, 0);
  pause();
}

void writeArduinoOnMatrix()
{
  boolean stopFlag = false;
  // here is the data for the characters
  byte a[5] = {
    B01111110,
    B10001000,
    B10001000,
    B10001000,
    B01111110
  };
  byte r[5] = {
    B00010000,
    B00100000,
    B00100000,
    B00010000,
    B00111110
  };
  byte d[5] = {
    B11111110,
    B00010010,
    B00100010,
    B00100010,
    B00011100
  };
  byte u[5] = {
    B00111110,
    B00000100,
    B00000010,
    B00000010,
    B00111100
  };
  byte i[5] = {
    B00000000,
    B00000010,
    B10111110,
    B00100010,
    B00000000
  };
  byte n[5] = {
    B00011110,
    B00100000,
    B00100000,
    B00010000,
    B00111110
  };
  byte o[5] = {
    B00011100,
    B00100010,
    B00100010,
    B00100010,
    B00011100
  };
  // now display them one by one with a small delay
  lc.setRow(0, 2, a[0]);
  lc.setRow(0, 3, a[1]);
  lc.setRow(0, 4, a[2]);
  lc.setRow(0, 5, a[3]);
  lc.setRow(0, 6, a[4]);
  delay(delaytime);
  lc.setRow(0, 2, r[0]);
  lc.setRow(0, 3, r[1]);
  lc.setRow(0, 4, r[2]);
  lc.setRow(0, 5, r[3]);
  lc.setRow(0, 6, r[4]);
  delay(delaytime);
  lc.setRow(0, 2, d[0]);
  lc.setRow(0, 3, d[1]);
  lc.setRow(0, 4, d[2]);
  lc.setRow(0, 5, d[3]);
  lc.setRow(0, 6, d[4]);
  delay(delaytime);
  lc.setRow(0, 2, u[0]);
  lc.setRow(0, 3, u[1]);
  lc.setRow(0, 4, u[2]);
  lc.setRow(0, 5, u[3]);
  lc.setRow(0, 6, u[4]);
  delay(delaytime);
  lc.setRow(0, 2, i[0]);
  lc.setRow(0, 3, i[1]);
  lc.setRow(0, 4, i[2]);
  lc.setRow(0, 5, i[3]);
  lc.setRow(0, 6, i[4]);
  delay(delaytime);
  lc.setRow(0, 2, n[0]);
  lc.setRow(0, 3, n[1]);
  lc.setRow(0, 4, n[2]);
  lc.setRow(0, 5, n[3]);
  lc.setRow(0, 6, n[4]);
  delay(delaytime);
  lc.setRow(0, 2, o[0]);
  lc.setRow(0, 3, o[1]);
  lc.setRow(0, 4, o[2]);
  lc.setRow(0, 5, o[3]);
  lc.setRow(0, 6, o[4]);
  delay(delaytime);
  lc.setRow(0, 2, 0);
  lc.setRow(0, 3, 0);
  lc.setRow(0, 4, 0);
  lc.setRow(0, 5, 0);
  lc.setRow(0, 6, 0);
  delay(delaytime);
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

void loop()
{
//  writeArduinoOnMatrix();
 habari();

  /*
    rows();
    columns();
    single();
    */
}

void pause()
{
    int reading;
  boolean stopFlag = false;

    while (stopFlag == false)
  {
    reading = digitalRead(buttonPin);
    if (reading != lastButtonState)
      lastDebounceTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      if (reading != buttonState)
      {
        buttonState = reading;
        if (buttonState == LOW)
          stopFlag = true;
      }
    }
    lastButtonState = reading;
  }
}

void ledPrint(char *message)
{
  byte b[8];
  for (int i = 0; i < strlen(message); i++)
  {
    for (int j = 0; j < 8; j++)
      b[j] = (message[i] == 32 ? alphabetArray[26][j] : alphabetArray[message[i] - 65][j]);
    for (int j = 0; j < 8; j++)
      lc.setRow(0, j, b[j]);
    delay(delaytime);
  }
}
