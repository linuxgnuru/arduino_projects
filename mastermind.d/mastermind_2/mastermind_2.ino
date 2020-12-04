/*
  Mastermind using LEDs

 Wiring:
 5v---*---10k----[LED]----GND
 * = analog connection
 7-segment LED Pins:
        ____
 A  1--|    |--8  H        --    H
 B  2--|    |--7  G       |  | A   G
 X     |    |     X        --    B
  GND--|    |     X       |  | C   E
 X     |    |     X        -- .  D   F (.)
 C  3--|    |--6  F (.)
 D  4--|____|--5  E

        HGFEDCBA
0out = 0b10000000
1out = 0b01000000
2out = 0b00100000
3out = 0b00010000
4out = 0b00001000
5out = 0b00000100
6out = 0b00000010
7out = 0b00000001
*/

int anaPin[] = {
  A0, A1, A2, A3
};

int aPin1 = A0;
int aPin2 = A1;
int aPin3 = A2;
int aPin4 = A3;

typedef enum {
  LED_B, // blue
  LED_R, // red
  LED_Y, // yellow
  LED_G, // green
  LED_W, // white
  LED_0  // nothing
}
led_enum;

//#define DEBUG 1

//Pin connected to DS of 74HC595
int dataPin = 2; // blue
//Pin connected to ST_CP of 74HC595
int latchPin = 3; // green
//Pin connected to SH_CP of 74HC595
int clockPin = 4; // yellow
// outside leads to ground and +5V
int raw = 0;           // variable to store the raw input value
int Vin = 5;           // variable to store the input voltage
float Vout = 0;        // variable to store the output voltage
float R1 = 10;         // variable to store the R1 value
//float R1 = 10000;         // variable to store the R1 value
//float R1 = 100;         // variable to store the R1 value
float R2 = 0;          // variable to store the R2 value
float buffer = 0;      // buffer variable for calculation

//              r,    b,    g,    y,    w
float highs[] = { -1,   -1,   -1,   -1,   -1 };   // high values
float lows[] =  { 1000, 1000, 1000, 1000, 1000 }; // low

int data1;
int data2;

void setup()
{
  int i;

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(latchPin, OUTPUT);
  //        11122233
  //        gyrgyrgy
  data1 = 0b00000000;
  //        3444
  //        rgyr
  data2 = 0b00000000;
  //    data1 |= 1 << 7; // 0b10000000
  for (i = 7; i > -1; i--)
  {
    data1 |= 1 << i;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
    delay(200);
    if (i != 5 && i != 2)
    {
      data1 &= ~(1 << i);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data1);
      shiftOut(dataPin, clockPin, data2);
      digitalWrite(latchPin, 1);
    }
  }
  data1 = 0b00100100;
  for (i = 7; i > 3; i--)
  {
    data2 |= 1 << i;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
    delay(200);
    if (i != 7 && i != 4)
    {
      data2 &= ~(1 << i);
      digitalWrite(latchPin, 0);
      shiftOut(dataPin, clockPin, data1);
      shiftOut(dataPin, clockPin, data2);
      digitalWrite(latchPin, 1);
    }
  }
#ifdef DEBUG
  Serial.begin(9600);             // Setup serial
#endif
  /*
    //  blinkAll_2Bytes(4, 250);
    // clear the shift register leds
    data1 = 0b00000000;
    data2 = 0b00000000;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
  */
  //calibrate();
}

int code[] = {
  LED_W, LED_G, LED_B, LED_R
};

int useR = 0;
int useB = 0;
int useY = 0;
int useG = 0;
int useW = 0;

int ans[] = {
  0, 0, 0, 0
};

int result[] = {
  LED_R, LED_R, LED_R, LED_R
};

int winner = 0;

void loop()
{
  int i;
  //  i = testPin(3);
  for (i = 0; i < 4; i++)
  {
    ans[i] = testPin(i);
  }
  for (i = 0; i < 4; i++)
  {
    switch (code[i])
    {
      case LED_B: useB = 1; break;
      case LED_R: useR = 1; break;
      case LED_G: useG = 1; break;
      case LED_W: useW = 1; break;
      case LED_Y: useY = 1; break;
    }
  }
  /*
  for (i = 0; i < 4; i++)
  {
    switch (ans[i])
    {
      case LED_R:
        digitalWrite(8, HIGH);
        for (i = 9; i < 13; i++)
          digitalWrite(i, LOW);
        break;
      case LED_B:
        digitalWrite(8, LOW);
        digitalWrite(9, HIGH);
        for (i = 10; i < 13; i++)
          digitalWrite(i, LOW);
        break;
      case LED_G:
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, HIGH);
        for (i = 11; i < 13; i++)
          digitalWrite(i, LOW);
        break;
      case LED_W:
        for (i = 8; i < 11; i++)
          digitalWrite(i, LOW);
        digitalWrite(11, HIGH);
        digitalWrite(12, LOW);
        break;
      case LED_Y:
        for (i = 8; i < 12; i++)
          digitalWrite(i, LOW);
        digitalWrite(12, HIGH);
        break;
      case LED_0:
        for (i = 8; i < 13; i++)
          digitalWrite(i, LOW);
        break;
    }
    delay(1000);
  }
  */

  // winner
  if (winner == 1 || (code[0] == ans[0] && code[1] == ans[1] && code[2] == ans[2] && code[3] == ans[3]))
  {
    data1 = 0b10010010;
    data2 = 0b01000000;
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
    for (i = 0; i < 3; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      // TODO; maybe find a way to blink the green LEDs?
    }
    winner = 1;
  }
  else
  {
    // test if nothing is plugged in
    for (i = 0; i < 4; i++)
    {
      if (ans[i] == LED_0)
        result[i] = LED_0;
    }
    // Turn on green
    if (code[0] == ans[0] || code[1] == ans[1] || code[2] == ans[2] || code[3] == ans[3])
    {
      if (code[0] == ans[0]) result[0] = LED_G;
      if (code[1] == ans[1]) result[1] = LED_G;
      if (code[2] == ans[2]) result[2] = LED_G;
      if (code[3] == ans[3]) result[3] = LED_G;
    }
    // find colors
    for (i = 0; i < 4; i++)
    {
      switch (ans[i])
      {
        case LED_B: if (result[i] != LED_G && useB == 1) result[i] = LED_Y; break;
        case LED_R: if (result[i] != LED_G && useR == 1) result[i] = LED_Y; break;
        case LED_G: if (result[i] != LED_G && useG == 1) result[i] = LED_Y; break;
        case LED_W: if (result[i] != LED_G && useW == 1) result[i] = LED_Y; break;
        case LED_Y: if (result[i] != LED_G && useY == 1) result[i] = LED_Y; break;
      }
    }
    for (i = 0; i < 4; i++)
    {
      switch (ans[i])
      {
        case LED_B: if (useB == 0) result[i] = LED_R; break;
        case LED_R: if (useR == 0) result[i] = LED_R; break;
        case LED_G: if (useG == 0) result[i] = LED_R; break;
        case LED_W: if (useW == 0) result[i] = LED_R; break;
        case LED_Y: if (useY == 0) result[i] = LED_R; break;
      }
    }
    for (i = 0; i < 4; i++)
    {
      changeBit(i, result[i]);
    }
    delay(1000);
  }
}

void calibrate()
{
  int i, j;
  for (i = 0; i < 5; i++)
  {
    for (j = 0; j < 10; j++)
    {
      raw = analogRead(i);    // Reads the Input PIN
      Vout = (5.0 / 1023.0) * raw;    // Calculates the Voltage on th Input PIN
      buffer = (Vin / Vout) - 1;
      R2 = R1 / buffer;
      if (lows[i] > R2)
        lows[i] = R2;
      if (highs[i] < R2)
        highs[i] = R2;
      delay(500);
    }
  }
  for (i = 0; i < 2; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }  
}

int testPin(int pin)
{
  int res;

  raw = analogRead(pin);    // Reads the Input PIN
  Vout = (5.0 / 1023.0) * raw;    // Calculates the Voltage on th Input PIN
  buffer = (Vin / Vout) - 1;
  R2 = R1 / buffer;
  /*
    raw = analogRead(pin);
    buffer = raw * Vin;
    Vout = (buffer) / 1024.0;
    buffer = (Vin/Vout) - 1;
    R2 = R1 * buffer;
  */
#ifdef DEBUG
  if (pin == 3)
  {
    Serial.print("Voltage: ");      //
    Serial.println(Vout);           // Outputs the information
    Serial.print("R2: ");           //
    Serial.println(R2);             //
  }
#endif
  // TODO:
  // maybe when first turned on; determine the following values?

  // calibrated
  if (R2 >= lows[4] && R2 <= highs[4]) res = LED_R;
  else if (R2 >= lows[3] && R2 <= highs[3]) res = LED_B;
  else if (R2 >= lows[2] && R2 <= highs[2]) res = LED_G;
  else if (R2 >= lows[1] && R2 <= highs[1]) res = LED_Y;
  else if (R2 >= lows[0] && R2 <= highs[0]) res = LED_W;
  else res = LED_0;

#if 0
  // USB rPi (100 ohm)
  if (R2 >= 10.40 && R2 <= 10.70) res = LED_W;
  else if (R2 >= 11.15 && R2 <= 11.45) res = LED_B;
  else if (R2 >= 11.46 && R2 <= 11.95) res = LED_G;
  else if (R2 >= 15.8 && R2 <= 16.5) res = LED_Y;
  else if (R2 >= 16.89 && R2 <= 17.10) res = LED_R;
  else res = LED_0;
#endif
#if 0
  // USB rPi (10k ohm)
  if (R2 >= 6.00 && R2 <= 6.29) res = LED_R;
  else if (R2 >= 6.30 && R2 <= 7.00) res = LED_Y;
  else if (R2 >= 9.23 && R2 <= 9.52) res = LED_G;
  else if (R2 >= 11.20 && R2 <= 11.90) res = LED_B;
  else if (R2 >= 12.28 && R2 <= 13.00) res = LED_W;
  else res = LED_0;
#endif
  // USB laptop
#if 0
  if (R2 >= 5.50 && R2 <= 5.8) res = LED_R;
  else if (R2 >= 5.98 && R2 <= 7) res = LED_Y;
  else if (R2 >= 8 && R2 <= 10) res = LED_G;
  else if (R2 >= 10.30 && R2 <= 10.49) res = LED_B;
  else if (R2 >= 10.50 && R2 <= 10.80) res = LED_W;
  else res = LED_0;
#endif
  // USB goalzero
#if 0
  if (R2 == 5.59) res = LED_R;
  else if (R2 == 5.96) res = LED_Y;
  else if (R2 == 8.47) res = LED_G;
  else if (R2 == 10.30) res = LED_B;
  else if (R2 == 10.50) res = LED_W;
  else res = LED_0;
#endif
  return res;
}

void changeBit(int pos, int color)
{
  /*
  data1 = 0b00000000;
  data2 = 0b00000000;
  */
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
          data1 |= 1 << 6;
          data1 &= ~(1 << 7);
          data1 &= ~(1 << 5);
          break;
        case LED_R:
          data1 |= 1 << 5;
          data1 &= ~(1 << 7);
          data1 &= ~(1 << 6);
          break;
        case LED_0:
          data1 &= ~(1 << 5);
          data1 &= ~(1 << 7);
          data1 &= ~(1 << 6);
          break;
      }
      //      Serial.println(data1);
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
          data1 |= 1 << 3;
          data1 &= ~(1 << 4);
          data1 &= ~(1 << 2);
          break;
        case LED_R:
          data1 |= 1 << 2;
          data1 &= ~(1 << 4);
          data1 &= ~(1 << 3);
          break;
        case LED_0:
          data1 &= ~(1 << 2);
          data1 &= ~(1 << 3);
          data1 &= ~(1 << 4);
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
          data1 |= 1 << 0;
          data1 &= ~(1 << 1);
          data2 &= ~(1 << 7);
          break;
        case LED_R:
          data2 |= 1 << 7;
          data1 &= ~(1 << 1);
          data1 &= ~(1 << 0);
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
          data2 |= 1 << 5;
          data2 &= ~(1 << 6);
          data2 &= ~(1 << 4);
          break;
        case LED_R:
          data2 |= 1 << 4;
          data2 &= ~(1 << 6);
          data2 &= ~(1 << 5);
          break;
        case LED_0:
          data2 &= ~(1 << 4);
          data2 &= ~(1 << 5);
          data2 &= ~(1 << 6);
          break;
      }
      break;
  }
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data1);
  shiftOut(dataPin, clockPin, data2);
  digitalWrite(latchPin, 1);
}

// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low
  //internal function setup
  int i = 0;
  int pinState;

  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    //if the value passed to myDataOut and ans[0] bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    pinState = (myDataOut & (1 << i) ? 1 : 0);
    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }
  //stop shifting
  digitalWrite(myClockPin, 0);
}

// blinks the whole register based on the number of times you want to blink "n" and the pause between them "d"
// starts with a moment of darkness to make sure the first blink has its full visual effect.
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

/*
  int ctr = 0;
  for (i = 0; i < 12; i++)
  {
    if (i < 9)
      data1 ^=
    else
    {
      data1 = 0b00000000;
      data2 |= 1 << ctr;
    }
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
    if (i < 9)
      data1 &= ~(1 << i);
    else
    {
      data2 &= ~(1 << ctr);
      ctr++;
    }
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
  }
*/
/*
  for (i = 0; i < 16; i++)
  {
    data1 = dataArray1[i];
    data2 = dataArray2[i];
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, data1);
    shiftOut(dataPin, clockPin, data2);
    digitalWrite(latchPin, 1);
    delay(500);
  }
*/

/*
  switch (testPin(aPin1))
  {
  case LED_R:
    digitalWrite(8, HIGH);
    for (i = 9; i < 13; i++)
      digitalWrite(i, LOW);
    break;
  case LED_B:
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
    for (i = 10; i < 13; i++)
      digitalWrite(i, LOW);
    break;
  case LED_G:
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, HIGH);
    for (i = 11; i < 13; i++)
      digitalWrite(i, LOW);
    break;
  case LED_W:
    for (i = 8; i < 11; i++)
      digitalWrite(i, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
    break;
  case LED_Y:
    for (i = 8; i < 12; i++)
      digitalWrite(i, LOW);
    digitalWrite(12, HIGH);
    break;
  case LED_0:
    for (i = 8; i < 13; i++)
      digitalWrite(i, LOW);
    break;
  default:  // Should never get here.
    for (i = 8; i < 13; i++)
      digitalWrite(i, HIGH);
    break;
  }
*/
/*
  data = digitArray[last];
  digitalWrite(latchPin, 0);
  //move 'em out
  shiftOut(dataPin, clockPin, data);
  //return the latch pin high to signal chip that it
  //no longer needs to listen for information
  digitalWrite(latchPin, 1);
*/

/*
  switch (testPin(A0))
  {
  case LED_R:
    Serial.println("red");
    break;
  case LED_B:
    Serial.println("blue");
    break;
  case LED_G:
    Serial.println("green");
    break;
  case LED_W:
    Serial.println("white");
    break;
  case LED_Y:
    Serial.println("yellow");
    break;
  case LED_0:
    Serial.println("nothing");
    break;
  }
*/

/*
byte dataArray1[] = {
  0b10000000, // g00
  0b01000000,
  0b00100000,
  0b00010000,
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001
};

byte dataArray2[] = {
  0b00001000,
  0b00000100,
  0b00000010,
  0b00000001,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};
*/

