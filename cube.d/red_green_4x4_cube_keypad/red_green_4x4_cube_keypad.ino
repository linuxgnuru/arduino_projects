/*
  keypad for testing
 1) row  (0 - 3)
 2) chip (0 - 1)
 3) led  (0 - 7)
 example:
 1 * 0 * 3
 
 col 
 0 1 2 3
 4 5 6 7
 8 9 A B
 C D E F 
 
 chip 0
 0 1 2 3
 4 5 6 7
 chip 1
 0 1 2 3
 4 5 6 7 
 
    *-*-*-*
   *-*-*-*|
  *-*-*-*||  row 3
 *-*-*-*|||
 |||*-*-*-*
 ||*-*-*-*|
 |*-*-*-*||  row 2
 *-*-*-*|||
 |||*-*-*-*
 ||*-*-*-*|
 |*-*-*-*||  row 1
 *-*-*-*|||
 |||*-*-*-*
 ||*-*-*-*
 |*-*-*-*    row 0
 *-*-*-*
 
 example from code
 
    *-*-*-* 0 B1111
   *-*-*-*  1 B1111
  *-*-*-*   2 B1111
 *-*-*-*    3 B1111
 
    .-.-.-* 0 B0001
   .-.-.-*  1 B0001
  .-.-.-*   2 B0001
 .-.-.-*    3 B0001
 
 // floor to wall
 // 
 B1111, B1111, B1111, B1111, // row 0
 B0000, B0000, B0000, B0000, // row 1
 B0000, B0000, B0000, B0000, // row 2
 B0000, B0000, B0000, B0000, // row 3
 10,
 B0111, B0111, B0111, B0111,
 B1000, B1000, B1000, B1000,
 B0000, B0000, B0000, B0000,
 B0000, B0000, B0000, B0000,
 10,
 B0011, B0011, B0011, B0011,
 B0100, B0100, B0100, B0100,
 B1000, B1000, B1000, B1000,
 B0000, B0000, B0000, B0000,
 10,
 B0001, B0001, B0001, B0001,
 B0010, B0010, B0010, B0010,
 B0100, B0100, B0100, B0100,
 B1000, B1000, B1000, B1000,
 10,
 B0001, B0001, B0001, B0001,
 B0001, B0001, B0001, B0001,
 B0010, B0010, B0010, B0010,
 B0100, B0100, B0100, B0100,
 10,
 B0001, B0001, B0001, B0001,
 B0001, B0001, B0001, B0001,
 B0001, B0001, B0001, B0001,
 B0010, B0010, B0010, B0010,
 10,
 B0001, B0001, B0001, B0001,
 B0001, B0001, B0001, B0001,
 B0001, B0001, B0001, B0001,
 B0001, B0001, B0001, B0001,
 10,
 
 
 */
#include <SPI.h>
#include <Keypad.h>
#include <avr/pgmspace.h> // allows use of PROGMEM to store patterns in flash

#define CUBESIZE 4
#define PLANESIZE CUBESIZE*CUBESIZE
#define PLANETIME 3333 // time each plane is displayed in us -> 100 Hz refresh
#define TIMECONST 20 // multiplies DisplayTime to get ms - why not =100?

const int ledPin = 12;
const int LATCH = 10;

const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = { 2, 3, 4, 5 }; //connect to row pinouts 
byte colPins[COLS] = { 6, 7, 8, 9 }; //connect to column pinouts

char keys[ROWS][COLS] = { 
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int planePin[] = { A5, A4, A3, A2 };

// LED Pattern Table in PROGMEM - last column is display time in 100ms units
// TODO this could be a lot more compact but not with binary pattern representation

PROGMEM const unsigned char patternTable[] = {
  B1111, B1111, B1111, B1111,
  B0000, B0000, B0000, B0000,
  B0000, B0000, B0000, B0000,
  B0000, B0000, B0000, B0000,
  10,
  B0111, B0111, B0111, B0111,
  B1000, B1000, B1000, B1000,
  B0000, B0000, B0000, B0000,
  B0000, B0000, B0000, B0000,
  10,
  B0011, B0011, B0011, B0011,
  B0100, B0100, B0100, B0100,
  B1000, B1000, B1000, B1000,
  B0000, B0000, B0000, B0000,
  10,
  B0001, B0001, B0001, B0001,
  B0010, B0010, B0010, B0010,
  B0100, B0100, B0100, B0100,
  B1000, B1000, B1000, B1000,
  10,
  B0001, B0001, B0001, B0001,
  B0001, B0001, B0001, B0001,
  B0010, B0010, B0010, B0010,
  B0100, B0100, B0100, B0100,
  10,
  B0001, B0001, B0001, B0001,
  B0001, B0001, B0001, B0001,
  B0001, B0001, B0001, B0001,
  B0010, B0010, B0010, B0010,
  10,
  B0001, B0001, B0001, B0001,
  B0001, B0001, B0001, B0001,
  B0001, B0001, B0001, B0001,
  B0001, B0001, B0001, B0001,
  10,

  // this is a dummy element for end of table (duration=0) aka !!!DO NOT TOUCH!!!
  B0000, B0000, B0000, B0000,
  B0000, B0000, B0000, B0000,
  B0000, B0000, B0000, B0000,
  B0000, B0000, B0000, B0000,
  0
};

byte data[] = { 0b00000000, 0b00000000 };

int keyCol = 0;
int keyRow = 0;
int keyChip = 0;
int keyLed = 0;
int keyPressNum = 0;

void setup()
{
  SPI.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, HIGH);
  for (int i = 0; i < CUBESIZE; i++)
  {
    pinMode(planePin[i], OUTPUT);
    digitalWrite(planePin[i], HIGH);
  }
  Serial.begin(9600);
  //  Serial.println(F("Enter chip num (0 / 1) and led (0-7)"));
  Serial.println(F("-= Begin =-"));
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00000000);
  SPI.transfer(0b00000000);
  digitalWrite(LATCH, HIGH);
}

#if 0
void loop()
{
  //  ledLoop();
  byte fu = 0b00000000;
  byte bar = 0b00000000;
  digitalWrite(planePin[3], LOW);
  digitalWrite(LATCH, LOW);
  SPI.transfer(0b00010001);
  SPI.transfer(0b00010001);
  digitalWrite(LATCH, HIGH);
  /*
  for (int i = 0; i < 8; i++)
   {
   digitalWrite(LATCH, LOW);
   SPI.transfer(fu);
   SPI.transfer(bar);
   digitalWrite(LATCH, HIGH);
   bitSet(fu, i);
   delay(50);
   }
   */
  delay(100);
}
#endif

//#if 0
void loop()
{
  char key = keypad.getKey();
  int chip = 0;
  int myCol = 0;

  //  while (Serial.available() > 0)
  if (key != NO_KEY)
  {
    Blip(1);
    //int val = Serial.read();
    //myRow = val - 48;
    //    Serial.print(F("key: "));
    myCol = key - 48;
    //    Serial.println(myCol);
    if (key - 48 < 10 && key - 48 > -1)
    {
      //  Serial.print(F("before - keyPressNum: "));
      //    Serial.println(keyPressNum);
      switch (keyPressNum++)
      {
      case 0: // row
        keyRow = key - 48;
        keyRow = constrain(keyRow, 0, 3);
        break;
      case 1: // chip
        keyChip = key - 48;
        keyChip = constrain(keyChip, 0, 1);
        break;
      case 2: // led
        keyLed = key - 48;
        keyLed = constrain(keyLed, 0, 7);
        break;
      default:
        break;
      }
      //  Serial.print(F("after - keyPressNum: "));
      //    Serial.println(keyPressNum);
    }
    else
    {
      //  Serial.print(F("Non-digit"));
      Blip(2);
      keyLed = keyChip = keyRow = 0;
      switch (key)
      {
      case 'A':    
        break;
      case 'B':    
        break;
      case 'C':    
        break;
      case 'D':    
        break;
      case '*':    
        break;
      case '#':    
        break;
      default:    
        break;
      }
      keyPressNum = 0;
      for (int z = 0; z < 4; z++)
        digitalWrite(planePin[z], HIGH);
      data[0] = data[1] = 0b00000000;
      /*
  digitalWrite(LATCH, LOW);
       SPI.transfer(0b00000000);
       SPI.transfer(0b00000000);
       digitalWrite(LATCH, HIGH);
       */
    }
    //    Serial.print(F("out of - keyPressNum: "));
    //    Serial.println(keyPressNum);
    if (keyPressNum == 3)
    {
      Serial.print(F("keyRow: "));
      Serial.print(keyRow);
      Serial.print(F(" keyChip: "));
      Serial.print(keyChip);
      Serial.print(F(" keyLed: "));
      Serial.println(keyLed);
      Blip(3);
      keyPressNum = 0;
      digitalWrite(planePin[keyRow], LOW);

      //  bitWrite(data[keyChip], keyLed, !bitRead(data[keyChip], keyLed));
      bitSet(data[keyChip], keyLed);

      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(LATCH, HIGH);
      /*
  Serial.print(F("Binary: "));
       Serial.print(data[0], BIN);
       Serial.print(F(" "));
       Serial.println(data[1], BIN);
       */
    }
  } // end key check
  /*
    chip = Serial.parseInt();
   myCol = Serial.parseInt();
   if (Serial.read() == '\n')
   {
   chip = constrain(chip, 0, 1);
   myCol = constrain(myCol, 0, 7);
   Serial.print("chip: ");
   Serial.print(chip);
   Serial.print(" col: ");
   Serial.println(myCol);
   Serial.print("data1 data2: ");
   Serial.print(data1, BIN);
   Serial.print(" ");
   Serial.println(data2, BIN);
   //bitSet((chip == 0 ? data1 : data2), myCol);
   bitWrite((chip == 0 ? data1 : data2), myCol, !bitRead((chip == 0 ? data1 : data2), myCol));
   digitalWrite(planePin[0], LOW);
   digitalWrite(LATCH, LOW);
   SPI.transfer(data1);
   SPI.transfer(data2);
   digitalWrite(LATCH, HIGH);
   digitalWrite(planePin[0], HIGH);
   Serial.println(F("Enter chip num (0 / 1) and led (0-7)"));
   }
   }
   delay(200);
   digitalWrite(planePin[1], HIGH);
   */
  delay(100);
}
//#endif

void ledLoop()
{
  int myRow, myCol;

  for (myRow = 0; myRow < CUBESIZE; myRow++)
  {
    digitalWrite(planePin[(myRow == 0 ? CUBESIZE - 1 : myRow - 1)], HIGH);
    digitalWrite(planePin[myRow], LOW);
    for (myCol = 0; myCol < PLANESIZE; myCol++)
    {
      //  bitSet(data[(myCol < 8 ? 1 : 0)], (myCol < 8 ? myCol : abs(myCol - 8)));
      if (myCol < 8)    bitSet(data[1], myCol);
      else    bitSet(data[0], abs(myCol - 8));
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(LATCH, HIGH);
      delay(90);
    }
    for (myCol = PLANESIZE; myCol > -1; myCol--)
    {
      //  bitClear(data[(myCol < 8 ? 1 : 0)], (myCol < 8 ? myCol : abs(myCol - 8)));
      //  /*
      if (myCol < 8)    bitClear(data[1], myCol);
      else    bitClear(data[0], abs(myCol - 8));
      //   */
      digitalWrite(LATCH, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(LATCH, HIGH);
      delay(90);
    }
    //    delay(250);
  }
}

void displayPat()
{
  // declare variables
  byte patternBuf[PLANESIZE]; // saves current pattern from PatternTable
  int patternIdx;
  byte displayTime; // time*100ms to display pattern
  unsigned long endTime;
  int plane; // loop counter for cube refresh
  int patbufidx; // indexes which byte from pattern buffer
  int ledrow; // counts LEDs in refresh loop
  int ledcol; // counts LEDs in refresh loop
  int ledpin; // counts LEDs in refresh loop
  int curPos;
  // Initialize PatternIdx to beginning of pattern table
  patternIdx = 0;
  // loop over entries in pattern table - while DisplayTime>0
  do
  {
    // read pattern from PROGMEM and save in array
    memcpy_P(patternBuf, patternTable + patternIdx, PLANESIZE);
    patternIdx += PLANESIZE;
    // read DisplayTime from PROGMEM and increment index
    displayTime = pgm_read_byte_near(patternTable + patternIdx++);
    // compute EndTime from current time (ms) and DisplayTime
    endTime = millis() + ((unsigned long) displayTime) * TIMECONST;
    // loop while DisplayTime>0 and current time < EndTime
    while (millis() < endTime)
    {
      patbufidx = 0; // reset index counter to beginning of buffer
      // loop over planes
      for (plane = 0; plane < CUBESIZE; plane++)
      {
        // turn previous plane off
        digitalWrite(planePin[(plane == 0 ? CUBESIZE : plane) - 1], HIGH);
        // load current plane pattern data into ports
        ledpin = 0;
        for (ledrow = 0; ledrow < CUBESIZE; ledrow++)
        {
          for (ledcol = 0; ledcol < CUBESIZE; ledcol++)
          {
            curPos = ledrow + ledcol;
            bitWrite(data[(curPos < 8 ? 1 : 0)], (curPos < 8 ? curPos : abs(curPos - 8)), patternBuf[patbufidx] & (1 << ledcol));
            digitalWrite(LATCH, LOW);
            SPI.transfer(data[1]);
            SPI.transfer(data[0]);
            digitalWrite(LATCH, HIGH);
            //digitalWrite( LEDPin[ledpin++], patternBuf[patbufidx] & (1 << ledcol) );
            //Serial.println(patternBuf[patbufidx] & (1 << ledcol));
          }
          patbufidx++;
        }
        // turn current plane on
        digitalWrite(planePin[plane], LOW);
        // delay PLANETIME us
        delayMicroseconds(PLANETIME);
      } // for plane
    } // while < endTime
  } while (displayTime > 0); // read patterns until time=0 which signals end
}

void Blip(int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(50);
    digitalWrite(ledPin, LOW);
    delay(50);
  }
}



