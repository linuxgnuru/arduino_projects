//We always have to include the library
#include <SPI.h>
#include "LedControl.h"
#include <string.h>
#include <ctype.h>

#include "font.h"

#define DEBUG

//#define DEMO_MODE
//#define SERIAL_INPUT
//#define ALL_CHARS
#define PR_MSG

// change the following based on how many shift register chips you are using
#define CHIPNUM 3

#define MYDELAYA 100
#define MYDELAYB 75
#define MAX_DELAY 250
#define MAX_LETTER 127
#define MAX_SZ 180

const byte LATCH = 10;

byte data[CHIPNUM];

// for SPI leds
boolean sequence1Done = false;
boolean sequence2Done = false;
unsigned long myDelay = 500;
unsigned long prevMillis = 0;
int ctr_a = 0;
int ctr_b = 0;
boolean goingRight = true;

boolean sequence1Done_max = false;
boolean sequence2Done_max = false;
unsigned long myDelay_max = 500;
unsigned long prevMillis_max = 0;
int ctr_a_max = 0;
int ctr_b_max = 0;

unsigned long prevM = 0;

// for printAll
int dl_printAll;
int dl_printAllcd;
int letterCtr_printAll = 32;
unsigned long prevMillis_printAll = 0;
unsigned long prevMillis_printAllcd = 0;

boolean sequenceDone_printAll = false;
boolean sequenceDone_printAllcd = false;

// for ledPrint
int dl_ledPrint;
int dl_ledPrintcd;
int letterCtr_ledPrint = 0;
unsigned long prevMillis_ledPrint = 0;
unsigned long prevMillis_ledPrintcd = 0;

boolean sequenceDone_ledPrint = false;
boolean sequenceDone_ledPrintcd = false;

/*
  Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
  pin 2 (12) is connected to the DataIn (blue)
  pin 3 (11) is connected to the CLK (yellow)
  pin 4 (10) is connected to LOAD (green)
  We have only a single MAX72XX.
*/
//                         B  Y  G
LedControl lc = LedControl(2, 3, 4, 1);

/*
  const byte buttonPin = 7;
  int  buttonState; // current reading from the input pin
  int  lastButtonState = LOW; // previous reading from the input pin
  long lastDebounceTime = 0; // last time the output pin was toggled
  long debounceDelay = 50; // debounce time; increase if the output flickers
*/

// we always wait a bit between updates of the display
unsigned long delaytime = 500;
char msg[MAX_SZ];

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  for (int i = 0; i < CHIPNUM; i++)
    data[i] = 0;
  //pinMode(buttonPin, INPUT_PULLUP);
  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, 8);
  // and clear the display
  lc.clearDisplay(0);
#ifdef DEBUG
  Serial.begin(9600);
#ifdef SERIAL_INPUT
  Serial.println("Ready for message.");
#endif
#endif
}

void loop()
{
  spiLoop();
#ifdef ALL_CHARS
  printAll();
#endif
#ifdef PR_MSG
  //  strcpy(msg, "Habari kwa Maisha na Maji!   Welcome to Maisha na Maji!");
  //  writeArduinoOnMatrix();
  //  habari();
  strcpy(msg, "Ghostbusters!!!   ");
  ledPrint();
#endif
#ifdef SERIAL_INPUT
  serialPrint();
#endif
#ifdef DEMO_MODE
  Single();
  Rows();
  Columns();
#endif
}

void printAll()
{
  unsigned long curMillis = millis();

  dl_printAll = delaytime - 250;
  dl_printAllcd = delaytime - 100;
  if ((curMillis - prevMillis_printAll >= dl_printAll) && sequenceDone_printAll == false)
  {
    prevMillis_printAll += dl_printAll;
    if (letterCtr_printAll >= MAX_LETTER)
    {
      sequenceDone_printAll = true;
      letterCtr_printAll = 32;
    }
    if (sequenceDone_printAll == false)
    {
      if (sequenceDone_printAllcd == true)
      {
        for (int i = 0; i < 8; i++)
          lc.setColumn(0, i, font[letterCtr_printAll][i]);
        letterCtr_printAll++;
        sequenceDone_printAllcd = false;
      }
      if ((curMillis - prevMillis_printAllcd >= dl_printAllcd) && sequenceDone_printAllcd == false)
      {
        prevMillis_printAllcd += dl_printAllcd;
        lc.clearDisplay(0);
        sequenceDone_printAllcd = true;
      }
    }
  }
  if (sequenceDone_printAll == true)
  {
    sequenceDone_printAllcd = false;
    sequenceDone_printAll = false;
    letterCtr_printAll = 32;
  }
}

void ledPrint()
{
  unsigned long curMillis = millis();

  dl_ledPrint = 100;//delaytime - 250;
  dl_ledPrintcd = 500;
  if ((curMillis - prevMillis_ledPrint >= dl_ledPrint) && sequenceDone_ledPrint == false)
  {
    prevMillis_ledPrint += dl_ledPrint;
    if (letterCtr_ledPrint >= strlen(msg))
    {
      sequenceDone_ledPrint = true;
      letterCtr_ledPrint = 0;
    }
    if (sequenceDone_ledPrint == false)
    {
      if (sequenceDone_ledPrintcd == true)
      {
        for (int i = 0; i < 8; i++)
          lc.setColumn(0, i, font[msg[letterCtr_ledPrint]][i]);
        letterCtr_ledPrint++;
        sequenceDone_ledPrintcd = false;
      }
      if ((curMillis - prevMillis_ledPrintcd >= dl_ledPrintcd) && sequenceDone_ledPrintcd == false)
      {
        prevMillis_ledPrintcd += dl_ledPrintcd;
        lc.clearDisplay(0);
        sequenceDone_ledPrintcd = true;
      }
    }
  }
  if (sequenceDone_ledPrint == true)
  {
    sequenceDone_ledPrintcd = false;
    sequenceDone_ledPrint = false;
    letterCtr_ledPrint = 0;
  }
}

#ifdef SERIAL_INPUT
void serialPrint()
{
  if (Serial.available())
  {
    if (readSerial() == true)
    {
      // lc.clearDisplay(0);
      // delay(delaytime);
      ledPrint();
    }
  }
}

int readSerial()
{
  char inChar;
  byte index = 0;
  boolean isData = false;

  // Don't read unless serial is ... well, available
  while (Serial.available() > 0)
  {
    // One less than the size of the array
    if (index < MAX_SZ - 1)
    {
      inChar = Serial.read(); // Read a character
      msg[index] = inChar; // Store it
      index++; // Increment where to write next
      msg[index] = '\0'; // Null terminate the string
      isData = true;
    }
  }
  return isData;
}
#endif

#ifdef DEMO_MODE
void Rows()
{
  for (int row = 0; row < 8; row++)
  {
    lc.setRow(0, row, B10100000);
    delay(delaytime);
    lc.setRow(0, row, (byte)0);
    delay(delaytime);
    for (int i = 0; i < row; i++)
    {
      lc.setRow(0, row, B10100000);
      delay(delaytime);
      lc.setRow(0, row, (byte)0);
      delay(delaytime);
    }
  }
}

void Columns()
{
  for (int col = 0; col < 8; col++)
  {
    delay(delaytime);
    lc.setColumn(0, col, B10100000);
    lc.setColumn(0, col, (byte)0);
    for (int i = 0; i < col; i++)
    {
      lc.setColumn(0, col, B10100000);
      delay(delaytime);
      lc.setColumn(0, col, (byte)0);
      delay(delaytime);
    }
    delay(delaytime);
  }
}

void Single()
{
  int d = 10;
  for (int row = 0; row < 8; row++)
  {
    for (int col = 0; col < 8; col++)
    {
      lc.setLed(0, row, col, true);
      for (int i = 0; i < col; i++)
      {
        lc.setLed(0, row, col, false);
        delay(d);
        lc.setLed(0, row, col, true);
        delay(d);
      }
      delay(d);
    }
  }
}
#endif

void spiLoop()
{
  unsigned long curMillis = millis();

  if (sequence1Done == false)
  {
    if (sequence2Done == true) // end first "for" loop
    {
      sequence2Done = false;
      if (++ctr_b > 1)
        sequence1Done = true;
    }
    else if ((curMillis - prevMillis >= myDelay) && sequence2Done == false)
    {
      prevMillis += myDelay;
      if (goingRight)
      {
        if (ctr_a > CHIPNUM * 8)
        {
          // end second "for" loop
          sequence2Done = true;
          ctr_a = 0;
        }
        if (sequence2Done == false)
          toggle(ctr_a++);
      }
      else
      {
        if (--ctr_a < -2)
        {
          // end second "for" loop
          sequence2Done = true;
          ctr_a = CHIPNUM * 8;
        }
        if (sequence2Done == false)
          toggle(ctr_a);
      }
    }
  }
  else
  {
    if (goingRight == true)
    {
      goingRight = false;
      ctr_a = CHIPNUM * 8;
    }
    else
    {
      goingRight = true;
      ctr_a = 0;
    }
    sequence1Done = false;
    ctr_b = 0;
  }
}

#if 0
void toggle(int input)
{
  int pin = input;
  myDelay = map(analogRead(A0), 0, 1023, 0, MAX_DELAY);
  //digitalWrite(pins[input], !digitalRead(pins[input]));
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < CHIPNUM; i++)
  {
    for (int j = 0; j < 8; j++)
      bitWrite(data[j], pin, !bitRead(data[j], pin));
    SPI.transfer(data[i]);
  }
  digitalWrite(LATCH, HIGH);
}
#endif

void toggle(int input)
{
  int pin, chip;
  
  myDelay = map(analogRead(A0), 0, 1023, 0, MAX_DELAY);
  // TODO make the down to just a couple of lines
  if (input < 8)
  {
    chip = CHIPNUM - 1;
    pin = input;
  }
  else if (input > 7 && input < 16)
  {
    chip = CHIPNUM - 2;
    pin = input - 8;
  }
  else if (input > 15 && input < 24)
  {
    chip = CHIPNUM - 3;
    pin = input - 16;
  }
  else if (input > 23 && input < 32)
  {
    chip = CHIPNUM - 4;
    pin = input - 24;
  }
  bitWrite(data[chip], pin, !bitRead(data[chip], pin));
  digitalWrite(LATCH, LOW);
  for (int i = 0; i < CHIPNUM; i++)
    SPI.transfer(data[i]);
  digitalWrite(LATCH, HIGH);
}

