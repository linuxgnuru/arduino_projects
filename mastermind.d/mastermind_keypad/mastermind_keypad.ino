/*
 Mastermind using a 4 x 4 keypad
 wiring for keypad: with pad facing you and starting from left to right; 
 +---+---+---+---+
 | 1 | 2 | 3 | A |
 +---+---+---+---+
 | 4 | 5 | 6 | B |
 +---+---+---+---+
 | 7 | 8 | 9 | C |
 +---+---+---+---+
 | * | 0 | # | D |
 +---+---+---+---+
 |               |
 +-+-+-+-+-+-+-+-+
 |2|3|4|5|6|7|8|9| (pins used)
 +-+-+-+-+-+-+-+-+
 | | | | | | | |
 + + + + + + + +
 */
#include <Keypad.h>

typedef enum {
  LED_R, // red
  LED_Y, // yellow
  LED_G, // green
  LED_0  // nothing
} 
led_enum;

// Defines
//#define DEBUG 1
#define NUM_LEDS 5

long winPreviousMillis = 0;
long losePreviousMillis = 0;

long interval = 300;

int max_tries = 10;

/*
 * Shift Register code:
 */
int dataPin  = 10; // Pin connected to DS of 74HC595 - blue
int latchPin = 11; //Pin connected to ST_CP of 74HC595 - green
int clockPin = 12; //Pin connected to SH_CP of 74HC595 - yellow

/*
 * Keypad code:
 */
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

// game mechanics
const char letters[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D' };

// Flags
int rFlag = false;
int resetFlag = false;
int firstFlag = true;
int winnerFlag = false;
int loserFlag = false;
int winSwapFlag = false;
int loseSwapFlag = false;

// this will be randomly generated
char pass[NUM_LEDS];
// users input
char answer[NUM_LEDS];
// result
int result[NUM_LEDS];
// what digits were picked
int used[14];

// for shift register leds:
int data1;
int data2;
int index = 0;
int ctr = 0;
int num_tries = 0;

void setup()
{
  firstFlag = true;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    answer[i] = 'E';
    result[i] = LED_0;
  }
  for (int i = 0; i < 14; i++)
  {
    used[i] = 0;
  }
  randomize();
  /*
  // The following is done in randomize (I think)
   for (int i = 0; i < 14; i++)
   {
   for (int j = 0; j < NUM_LEDS; j++)
   {
   if (letters[i] == pass[j])
   used[i] = 1;
   }
   }
   */
  // g2r - green to red
  g2r(150);
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY && (key == '*' || key == '#'))
  {
    if (loserFlag == false)
    {
      index = ctr = 0;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        result[i] = LED_0;
        answer[i] = 'E';
        changeBit(i, LED_0);
      }
      if (key == '*')
      {
        blip(2, 100);
        rFlag = true;
#ifdef DEBUG
        Serial.println("-- clear --");
#endif
      }
    }
    if (key == '#')
    {
      blip(5, 100);
      winnerFlag = false;
      resetFlag = true;
      num_tries = 0;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        result[i] = LED_0;
      }
      for (int i = 0; i < 14; i++)
      {
        used[i] = 0;
      }
      randomize();
    }
  }
  // num tries is never incremented so you'll never lose right now...
  if (num_tries > max_tries)// && loserFlag == true)
  {
    unsigned long loseCurrentMillis = millis();
    loserFlag = true;
    winnerFlag = false;
    if (loseSwapFlag == false)
    {
      data1 = 0b00100100;
      data2 = 0b10010010;
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data1);
      shiftOut(dataPin, clockPin, data2);
      digitalWrite(latchPin, 1);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      data1 = 0b01001001;
      data2 = 0b00100100;
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data1);
      shiftOut(dataPin, clockPin, data2);
      digitalWrite(latchPin, 1);
      digitalWrite(LED_BUILTIN, LOW);
    }
    if (resetFlag == true || loseCurrentMillis - losePreviousMillis > interval)
    {
      losePreviousMillis = loseCurrentMillis;
      loseSwapFlag = (loseSwapFlag == false ? true : false);
    }
  }
  else
  {
    if (firstFlag == true)
    {
      firstFlag = false;
      blip(2, 150);
    }
    if (key != NO_KEY && winnerFlag == false && (key != '*' && key != '#'))
    {
      if (answer[index - 1] != key)
      {
        blip(1, 100);
        answer[index++] = key;
#ifdef DEBUG
        Serial.print(num_tries);
        Serial.print(" - ");
        Serial.print("[");
        Serial.print(index - 1);
        Serial.print(":");
        Serial.print(key);
        Serial.println("]");
#endif
      }
    }
    // we've gotten all the digits we need buttons have been pressed
    if (index == NUM_LEDS && winnerFlag == false)// && (key != '*' && key != '#'))
    {
      index = 0;
      // test for a correct code
      for (int i = 0; i < NUM_LEDS; i++)
      {
        if (pass[i] == answer[i])
        {
          ctr++;
          result[i] = LED_G;
        }
      }
      if (ctr >= NUM_LEDS)
      {
        winnerFlag = true;
      }
      else
      {
        ctr = 0;
        for (int i = 0; i < NUM_LEDS; i++)
        {
          if ((result[i] != LED_G) && used[whatchar(answer[i])] == 1)
          {
            result[i] = LED_Y;
          }
          if (result[i] == LED_0)
          {
            result[i] = LED_R;
          }
        }
        num_tries++;
      } // end else (not winner)
    } // end if (NUM_LEDS buttons check
    // winner
    if (winnerFlag == true)
    {
      unsigned long winCurrentMillis = millis();
      if (winSwapFlag == false)
      {
        data1 = 0b10010010;
        data2 = 0b01001000;
        digitalWrite(latchPin, 0);
        shiftOut(dataPin, clockPin, data1);
        shiftOut(dataPin, clockPin, data2);
        digitalWrite(latchPin, 1);
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        data1 = 0b01001001;
        data2 = 0b00100100;
        digitalWrite(latchPin, 0);
        shiftOut(dataPin, clockPin, data1);
        shiftOut(dataPin, clockPin, data2);
        digitalWrite(latchPin, 1);
        digitalWrite(LED_BUILTIN, LOW);
      }
      if (resetFlag == true || winCurrentMillis - winPreviousMillis > interval)
      {
        winPreviousMillis = winCurrentMillis;
        winSwapFlag = (winSwapFlag == false ? true : false);
      }
    }
    else
    {
      /*
      if (result[0] != 3)
      {
        Serial.print("result: ");
        Serial.println(result[0]);
      }
      */
      if (result[0] != 3)// && (key != '*' && key != '#'))
      {
#ifdef DEBUG
        /* MASSIVE DEBUGGING STUFF */
        if (Serial.available())
        {
          int val = Serial.read();
          switch (val)
          {
          case '?': 
          case 'H': 
          case 'h':
            Serial.println("");
            Serial.println("help");
            Serial.println("----");
            Serial.println("p - password / answer");
            Serial.println("r - result (just digits)");
            Serial.println("c - result (color name)");
            Serial.println("u - used");
            Serial.println("l - letters / used");
            Serial.println("h/? - this list");
            break;
          case 'P': 
          case 'p': 
          case 'R': 
          case 'r':
            Serial.println("");
            Serial.print("pass: [");
            for (int i = 0; i < NUM_LEDS; i++)
              Serial.print(pass[i]);
            Serial.print("] answer: [");
            for (int i = 0; i < NUM_LEDS; i++)
              Serial.print(answer[i]);
            Serial.println("]");
            if (val == 'R' || val == 'r')
            {
              Serial.print("result: ");
              for (int i = 0; i < NUM_LEDS; i++)
              {
                Serial.print(result[i]);
                Serial.print(" ");
              }
              Serial.println("");
            }
            break;
          case 'C': 
          case 'c':
            Serial.print("result: ");
            for (int i = 0; i < NUM_LEDS; i++)
            {
              Serial.print(print_color(result[i]));
              Serial.print(" ");
            }
            Serial.println("");
            break;
          case 'U': 
          case 'u':
            Serial.println("");
            Serial.println("used:");
            for (int i = 0; i < 14; i++)
            {
              Serial.print(i);
              Serial.print(" ");
            }
            Serial.println("");
            for (int i = 0; i < 14; i++)
            {
              Serial.print(used[i]);
              Serial.print(" ");
            }
            Serial.println("");
            break;
          case 'L': 
          case 'l':
            Serial.println("");
            Serial.println("letters:");
            for (int i = 0; i < 14; i++)
            {
              Serial.print("[");
              if (i < 10)
                Serial.print("0");
              Serial.print(i);
              Serial.print("]: ");
              Serial.print(letters[i]);
              Serial.print(" ");
              Serial.println(used[i]);
            }
            Serial.println("");
            break;
          }
        }
#endif
        for (int i = 0; i < NUM_LEDS; i++)
          changeBit(i, result[i]);
      }
      else
        for (int i = 0; i < NUM_LEDS; i++)
          changeBit(i, LED_0);
    }
  }
  delay(100);
}

void randomize()
{
  int ran;
  int break_flag = false;
  int idx = 0;

  randomSeed(analogRead(A0));
  for (int i = 0; i < 14; i++)
    used[i] = 0;
  while (break_flag == false)
  {
    ran = random(0, 14);
    if (used[ran] == 0)
    {
      used[ran] = 1;
      pass[idx++] = letters[ran];
    }
    if (idx == NUM_LEDS)
      break_flag = true;
  }
  Serial.println("--NEW--");
  Serial.print("[PASSCODE: ");
  for (int i = 0; i < NUM_LEDS; i++)
    Serial.print(pass[i]);
  Serial.println("]");
}

char *print_color(int color)
{
  char *ret;
  switch (color)
  {
    case LED_R: ret = "red"; break;
    case LED_G: ret = "green"; break;
    case LED_Y: ret = "yellow"; break;
    case LED_0: ret = "off"; break;
  }
  return ret;
}

int whatchar(char k)
{
  int num = 0;
  switch (k)
  {
    case '0': num = 0; break;
    case '1': num = 1; break;
    case '2': num = 2; break;
    case '3': num = 3; break;
    case '4': num = 4; break;
    case '5': num = 5; break;
    case '6': num = 6; break;
    case '7': num = 7; break;
    case '8': num = 8; break;
    case '9': num = 9; break;
    case 'A': num = 10; break;
    case 'B': num = 11; break;
    case 'C': num = 12; break;
    case 'D': num = 13; break;
  }
  return num;
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int i = 0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    pinState = (myDataOut & (1<<i) ? 1 : 0);
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}

void blinkAll_2Bytes(int n, int d)
{
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++)
  {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}

void g2r(int d)
{
  /*
    data1    data2
    76543210 76543210
    00000000 00000000 // all off
    GYRGYRGY RGYRGYR

    00000000 00000000 // all off
    76543210 76543210
    10010010 01001000 // all green d1 = 7, 4, 1; d2 = 6, 3
    76543210 76543210
    01001001 00100100 // all yellow d1 = 6, 3, 0; d2 = 5, 2
    76543210 76543210
    00100100 10010000 // all red d1 = 5, 2; d2 = 7, 4
  */
  int d1 = 0b00000000;
  int d2 = 0b00000000;
  for (int i = 7; i > -1; i--)
  {
    bitSet(d1, i);
//    d1 |= 1 << i;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, d1);
    shiftOut(dataPin, clockPin, d2);
    digitalWrite(latchPin, 1);
    delay(d);
    if (i != 5 && i != 2)
    {
      bitClear(d1, i);
//      d1 &= ~(1 << i);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, d1);
      shiftOut(dataPin, clockPin, d2);
      digitalWrite(latchPin, 1);
    }
  }
  d1 = 0b00100100;
  for (int i = 7; i > 0; i--)
  {
    bitSet(d2, i);
//    d2 |= 1 << i;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, d1);
    shiftOut(dataPin, clockPin, d2);
    digitalWrite(latchPin, 1);
    delay(d);
    if (i != 7 && i != 4 && i != 1)
    {
      bitClear(d2, i);
//      d2 &= ~(1 << i);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, d1);
      shiftOut(dataPin, clockPin, d2);
      digitalWrite(latchPin, 1);
    }
  }
}

void blip(int times, int dela)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(dela);
    digitalWrite(LED_BUILTIN, LOW);
    delay(dela);
  }
}

void changeBit(int pos, int color)
{
  switch (pos)
  {
  case 0: // 7 6 5
    switch (color)
    {
    case LED_G: // 7
      bitSet(data1, 7);
      bitClear(data1, 6);
      bitClear(data1, 5);
      break;
    case LED_Y: // 6
      bitClear(data1, 7);
      bitSet(data1, 6);
      bitClear(data1, 5);
      break;
    case LED_R: // 5
      bitClear(data1, 7);
      bitClear(data1, 6);
      bitSet(data1, 5);
      break;
    case LED_0:
      for (int i = 5; i < 8; i++)
        bitClear(data1, i);
      break;
    }
    break;
  case 1: // 4 3 2
    switch (color)
    {
    case LED_G: // 4
      bitSet(data1, 4);
      bitClear(data1, 3);
      bitClear(data1, 2);
      break;
    case LED_Y: // 3
      bitClear(data1, 4);
      bitSet(data1, 3);
      bitClear(data1, 2);
      break;
    case LED_R: // 2
      bitClear(data1, 4);
      bitClear(data1, 3);
      bitSet(data1, 2);
      break;
    case LED_0:
      for (int i = 2; i < 5; i++)
        bitClear(data1, i);
      break;
    }
    break;
  case 2: // d1 1 0 d2 7
    switch (color)
    {
    case LED_G: // d1 1
      bitSet(data1, 1);
      bitClear(data1, 0);
      bitClear(data2, 7);
      break;
    case LED_Y: // d1 0
      bitClear(data1, 1);
      bitSet(data1, 0);
      bitClear(data2, 7);
      break;
    case LED_R: // d2 7
      bitClear(data1, 1);
      bitClear(data1, 0);
      bitSet(data2, 7);
      break;
    case LED_0:
      bitClear(data1, 1);
      bitClear(data1, 0);
      bitClear(data2, 7);
      break;
    }
    break;
  case 3: // d2 6 5 4
    switch (color)
    {
    case LED_G: // d2 6
      bitSet(data2, 6);
      bitClear(data2, 5);
      bitClear(data2, 4);
      break;
    case LED_Y: // d2 5
      bitClear(data2, 6);
      bitSet(data2, 5);
      bitClear(data2, 4);
      break;
    case LED_R: // d2 4
      bitClear(data2, 6);
      bitClear(data2, 5);
      bitSet(data2, 4);
      break;
    case LED_0:
      for (int i = 4; i < 7; i++)
        bitClear(data2, i);
      break;
    }
  case 4: // d2 3 2 1
    switch (color)
    {
    case LED_G: // d2 3
      bitSet(data2, 3);
      bitClear(data2, 2);
      bitClear(data2, 1);
      break;
    case LED_Y: // d2 2
      bitClear(data2, 3);
      bitSet(data2, 2);
      bitClear(data2, 1);
      break;
    case LED_R: // d2 1
      bitClear(data2, 3);
      bitClear(data2, 2);
      bitSet(data2, 1);
      break;
    case LED_0:
      for (int i = 1; i < 4; i++)
        bitClear(data2, i);
      break;
    }
    break;
  }
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data1);
  shiftOut(dataPin, clockPin, data2);
  digitalWrite(latchPin, 1);
}

/*
  if (Serial.available())
 {
 int val = Serial.read();
 if (val == 'V')
 {
 Serial.println("-= Displaying internal volt =-");
 command = CMD_VOLT;
 }
 if (val == 'R')
 {
 Serial.println("-= Displaying resistance =-");
 command = CMD_RES;
 }
 if (val == 'C')
 {
 Serial.println("-= Displaying color names =-");
 command = CMD_COL;
 }
 if (val == 'N')
 {
 Serial.println("-= Turning off display =-");
 command = CMD_NULL;
 }
 }
 if (command == CMD_COL)    Serial.println(colorNames[ans[0]]);
 */

/*
    // Turn on green
 if (code[0] == ans[0] || code[1] == ans[1] || code[2] == ans[2] || code[3] == ans[3])
 {
 if (code[0] == ans[0]) result[0] = LED_G;
 if (code[1] == ans[1]) result[1] = LED_G;
 if (code[2] == ans[2]) result[2] = LED_G;
 if (code[3] == ans[3]) result[3] = LED_G;
 }
 */

/*
  for (int z = 0; z < 4; z++)
 {
 Serial.println(pass[z]);
 delay(1000);
 }
 n = sizeof(code) / sizeof(code[0]);
 //  srand(time(NULL));
 for (i = n - 1; i > 0; i--)
 {
 j = random(1, i + 1);
 //    j = rand() % (i + 1);
 swap(&code[i], &code[j]);
 }
 */
#if 0
int num;
Serial.println("");
for (int i = 0; i < NUM_LEDS; i++)
{
  // now test to see if a column should be yellow or red
  Serial.print(i);
  Serial.print(": ");
  Serial.print(pass[i]);
  Serial.println("");
  for (int j = 0; j < NUM_LEDS; j++)
  {
    if (answer[j] == pass[i] || used[num] == 1)
    {
      if (answer[j] == pass[i])
      {
        Serial.println("green");
        result[i] = LED_G;
      }
      if (used[num] == 1 && result[i] != LED_G)
      {
        Serial.println("yellow");
        result[i] = LED_Y;
      }
    }
    else
      result[i] = LED_R;
    Serial.print(answer[j]);
    Serial.print(" ");
    Serial.print(used[num]);
    Serial.print(" ");
    /*
              Serial.print("ans / pass: ");
     Serial.print(answer[j]); Serial.print(" / "); Serial.println(pass[i]);
     Serial.print("used: ["); Serial.print(num);
     Serial.print("]: "); Serial.println(used[num]);
     */
  } // end for j (answer)
  Serial.println("");
} // end for i
Serial.print("result: ");
for (int i = 0; i < NUM_LEDS; i++)
{
  Serial.print(result[i]);
  Serial.print(" ");
}
Serial.println("");


void changeBit(int pos, int color)
{
  switch (pos)
  {
  case 0:
    switch (color)
    {
    case LED_G:
      data1 |= 1 << 7;
      data1 &= ~(1 << 6);
      data1 &= ~(1 << 5);
      break;
    case LED_Y:
      data1 &= ~(1 << 7);
      data1 |= 1 << 6;
      data1 &= ~(1 << 5);
      break;
    case LED_R:
      data1 &= ~(1 << 7);
      data1 &= ~(1 << 6);
      data1 |= 1 << 5;
      break;
    case LED_0:
      data1 &= ~(1 << 7);
      data1 &= ~(1 << 6);
      data1 &= ~(1 << 5);
      break;
    }
    break;
  case 1:
    switch (color)
    {
    case LED_G:
      data1 |= 1 << 4;
      data1 &= ~(1 << 3);
      data1 &= ~(1 << 2);
      break;
    case LED_Y:
      data1 &= ~(1 << 4);
      data1 |= 1 << 3;
      data1 &= ~(1 << 2);
      break;
    case LED_R:
      data1 &= ~(1 << 4);
      data1 &= ~(1 << 3);
      data1 |= 1 << 2;
      break;
    case LED_0:
      data1 &= ~(1 << 4);
      data1 &= ~(1 << 3);
      data1 &= ~(1 << 2);
      break;
    }
    break;
  case 2:
    switch (color)
    {
    case LED_G:
      data1 |= 1 << 1;
      data1 &= ~(1 << 0);
      data2 &= ~(1 << 7);
      break;
    case LED_Y:
      data1 &= ~(1 << 1);
      data1 |= 1 << 0;
      data2 &= ~(1 << 7);
      break;
    case LED_R:
      data1 &= ~(1 << 1);
      data1 &= ~(1 << 0);
      data2 |= 1 << 7;
      break;
    case LED_0:
      data1 &= ~(1 << 1);
      data1 &= ~(1 << 0);
      data2 &= ~(1 << 7);
      break;
    }
    break;
  case 3:
    switch (color)
    {
    case LED_G:
      data2 |= 1 << 6;
      data2 &= ~(1 << 5);
      data2 &= ~(1 << 4);
      break;
    case LED_Y:
      data2 &= ~(1 << 6);
      data2 |= 1 << 5;
      data2 &= ~(1 << 4);
      break;
    case LED_R:
      data2 &= ~(1 << 6);
      data2 &= ~(1 << 5);
      data2 |= 1 << 4;
      break;
    case LED_0:
      data2 &= ~(1 << 6);
      data2 &= ~(1 << 5);
      data2 &= ~(1 << 4);
      break;
    }
  case 4:
    switch (color)
    {
    case LED_G:
      data2 |= 1 << 3;
      data2 &= ~(1 << 2);
      data2 &= ~(1 << 1);
      break;
    case LED_Y:
      data2 &= ~(1 << 3);
      data2 |= 1 << 2;
      data2 &= ~(1 << 1);
      break;
    case LED_R:
      data2 &= ~(1 << 3);
      data2 &= ~(1 << 2);
      data2 |= 1 << 1;
      break;
    case LED_0:
      data2 &= ~(1 << 3);
      data2 &= ~(1 << 2);
      data2 &= ~(1 << 1);
      break;
    }
    break;
  }
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data1);
  shiftOut(dataPin, clockPin, data2);
  digitalWrite(latchPin, 1);
}

#endif


