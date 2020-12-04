#define DEBUG 1

#ifdef DEBUG
int maximum = 0; //declare and initialize maximum as zero
int minimum = 1023; //declare and initialize minimum as 1023
//int track = 0; //variable to keep track
#endif

const int anaPin = A0;

int VU;

void setup()
{
#ifdef DEBUG
  Serial.begin(9600); //set baud rate
  //Serial.println(F("----=[ vu meter ]=---"));
#endif
}

// 898 115
// 853 297
void loop()
{
#ifdef DEBUG
  //record the highest value recieved on A5
  if (analogRead(anaPin) > maximum)
    maximum = analogRead(anaPin);
  //record the lowest value recieved on A5
  if (analogRead(anaPin) < minimum)
    minimum = analogRead(anaPin);

  //increase track by 1 after every iteration
  //track++;

  /*
    display both the maximum and minimum value after 5 second
    track is used to determine the time interval it takes for
    the program to display the maximum and minimum values
    e.g. here i use 1000 so as to display the min and max values
    after every 5 second
  */
  //if (track == 100)  {
    Serial.print(analogRead(anaPin));
    //Serial.print(F("Maximum:\t"));
    Serial.print(F("\t"));
    Serial.print(maximum);
    //Serial.print(F("Minimum:\t"));
    Serial.print(F("\t"));
    Serial.println(minimum);
    //track = 0; //set back track to zero
 // delay(500);
 // }
#else
  VU = analogRead(anaPin);
  /*
    int nv = map(VU, 115, 898, 0, 23);
    int chipnum = nv / 8;
    int lednum = nv % 8;
  */
  // 531 559
  // 246 896
  if (VU == 0)
    data[0] = data[1] = 0b00000000;
  if (VU > 531 && VU < 559)// || (VU > 509 && VU < 580))
  {
    Serial.println(F("0"));
  }
  else if (VU > 509 && VU < 580)
  {
    Serial.println(F("1"));
  }
  else if (VU > 488 && VU < 601)
  {
    data[0] = 0b00000000;
    data[1] = 0b00000011;
  }
  else if (VU > 467 && VU < 622)
  {
    data[0] = 0b00000000;
    data[1] = 0b00000111;
  }
  else if (VU > 445 && VU < 644)
  {
    data[0] = 0b00000000;
    data[1] = 0b00001111;
  }
  else if (VU > 424 && VU < 665)
  {
    data[0] = 0b00000000;
    data[1] = 0b00011111;
  }
  else if (VU > 403 && VU < 686)
  {
    data[0] = 0b00000000;
    data[1] = 0b00111111;
  }
  else if (VU > 381 && VU < 708)
  {
    data[0] = 0b00000000;
    data[1] = 0b01111111;
  }
  else if (VU > 360 && VU < 729)
  {
    data[0] = 0b00000000;
    data[1] = 0b11111111;
  }
  else if (VU > 339 && VU < 750)
  {
    data[0] = 0b00000001;
    data[1] = 0b11111111;
  }
  else if (VU > 317 && VU < 772)
  {
    data[0] = 0b00000011;
    data[1] = 0b11111111;
  }
  else if (VU > 296 && VU < 793)
  {
    data[0] = 0b00000111;
    data[1] = 0b11111111;
  }
  else if (VU > 275 && VU < 814)
  {
    data[0] = 0b00001111;
    data[1] = 0b11111111;
  }
  else if (VU > 253 && VU < 836)
  {
    data[0] = 0b00011111;
    data[1] = 0b11111111;
  }
  else if (VU > 232 && VU < 857)
  {
    data[0] = 0b00111111;
    data[1] = 0b11111111;
  }
  else if (VU < 211 || VU > 878)
  {
    data[0] = 0b11111111;
    data[1] = 0b11111111;
  }
  /*
    if (VU > 542 && VU < 530)
    {
      data[0] = 0b00000000;
      data[1] = 0b00000001;
    }
    else if (VU > 519 && VU < 559)
    {
      data[0] = 0b00000000;
      data[1] = 0b00000011;
    }
    else if (VU > 497 && VU < 581)
    {
      data[0] = 0b00000000;
      data[1] = 0b00000111;
    }
    else if (VU > 476 && VU < 602)
    {
      data[0] = 0b00000000;
      data[1] = 0b00001111;
    }
    else if (VU > 455 && VU < 623)
    {
      data[0] = 0b00000000;
      data[1] = 0b00011111;
    }
    else if (VU > 433 && VU < 645)
    {
      data[0] = 0b00000000;
      data[1] = 0b00111111;
    }
    else if (VU > 412 && VU < 666)
    {
      data[0] = 0b00000000;
      data[1] = 0b01111111;
    }
    else if (VU > 391 && VU < 687)
    {
      data[0] = 0b00000000;
      data[1] = 0b11111111;
    }
    else if (VU > 369 && VU < 709)
    {
      data[0] = 0b00000001;
      data[1] = 0b11111111;
    }
    else if (VU > 348 && VU < 730)
    {
      data[0] = 0b00000011;
      data[1] = 0b11111111;
    }
    else if (VU > 327 && VU < 751)
    {
      data[0] = 0b00000111;
      data[1] = 0b11111111;
    }
    else if (VU > 305 && VU < 773)
    {
      data[0] = 0b00001111;
      data[1] = 0b11111111;
    }
    else if (VU > 284 && VU < 794)
    {
      data[0] = 0b00011111;
      data[1] = 0b11111111;
    }
    else if (VU > 263 && VU < 815)
    {
      data[0] = 0b00111111;
      data[1] = 0b11111111;
    }
    else if (VU > 263 && VU < 815)
    {
      data[0] = 0b01111111;
      data[1] = 0b11111111;
    }
    else if (VU < 242 || VU > 837)
    {
      data[0] = 0b11111111;
      data[1] = 0b11111111;
    }
  */
  /*
    if (VU > 552 && VU < 596)
    {
      data[0] = 0b00000000;
      data[1] = 0b00000111;
    }
    else if (VU > 489 && VU < 660)
    {
      data[0] = 0b00000000;
      data[1] = 0b00011111;
    }
    else if (VU > 425 && VU < 725)
    {
      data[0] = 0b00000000;
      data[1] = 0b11111111;
    }
    else if (VU > 361 && VU < 789)
    {
      data[0] = 0b00000111;
      data[1] = 0b11111111;
    }
    else if (VU > 297 && VU < 853)
    {
      data[0] = 0b00011111;
      data[1] = 0b11111111;
    }
    else if (VU < 297 || VU > 853)
    {
      data[0] = 0b11111111;
      data[1] = 0b11111111;
    }
  */
#endif
}
