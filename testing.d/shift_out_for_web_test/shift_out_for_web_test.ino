const byte LATCHp = 10; // green
const byte DATAp = 9; // blue
const byte CLOCKp = 8; // yellow

unsigned long lastM = 0;

byte data = 0b00000000;

int ctr = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LATCHp, OUTPUT);
  pinMode(DATAp, OUTPUT);
  pinMode(CLOCKp, OUTPUT);
}

void loop()
{
  switch (ctr++)
  {
    case 0:
      // All on, all off
      oneAfterAnother();
      break;
    case 1:
      // Scroll down the line
      oneOnAtATime();
      break;
    case 2:
      // Like above, but back and forth
      pingPong();
      break;
    case 3:
      // Blink random LEDs
      randomLED();
      break;
    case 4:
      marquee();
      break;
    case 5:
      // Bit patterns from 0 to 255
      binaryCount();
      break;
    default:
      ctr = 0;
      break;
  }

#if 0
  unsigned long curM = millis();
  if (curM - lastM >= 1000)
  {
    lastM = curM;
    for (int ctr = 0; ctr < 8; ctr++)
    {
      bitSet(data, ctr);
      digitalWrite(LATCHp, LOW);

      digitalWrite(LATCHp, HIGH);
    }
    for (int ctr = 7; ctr > -1 ; ctr--)
    {
      bitClear(data, ctr);
      digitalWrite(LATCH, LOW);

      digitalWrite(LATCH, HIGH);
    }
  }
#endif
}

void shiftWrite(int desiredPin, boolean desiredState)
{
  bitWrite(data, desiredPin, desiredState);
  shiftOut(DATAp, CLOCKp, MSBFIRST, data);
  digitalWrite(LATCHp, HIGH);
  digitalWrite(LATCHp, LOW);
}

void oneAfterAnother()
{
  int index;
  int delayTime = 100; // Time (milliseconds) to pause between LEDs
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);
    delay(delayTime);
  }
  for (index = 7; index >= 0; index--)
  {
    shiftWrite(index, LOW);
    delay(delayTime);
  }
}

void oneOnAtATime()
{
  int index;
  int delayTime = 100; // Time (milliseconds) to pause between LEDs
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);  // turn LED on
    delay(delayTime);   // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
}

void pingPong()
{
  int index;
  int delayTime = 100; // time (milliseconds) to pause between LEDs
  for (index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH);  // turn LED on
    delay(delayTime);   // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
  for (index = 7; index >= 0; index--)
  {
    shiftWrite(index, HIGH);  // turn LED on
    delay(delayTime);   // pause to slow down the sequence
    shiftWrite(index, LOW); // turn LED off
  }
}

void randomLED()
{
  int index;
  int delayTime = 100; // time (milliseconds) to pause between LEDs
  index = random(8);  // pick a random number between 0 and 7
  shiftWrite(index, HIGH);  // turn LED on
  delay(delayTime);   // pause to slow down the sequence
  shiftWrite(index, LOW); // turn LED off
}

void marquee()
{
  int index;
  int delayTime = 200; // Time (milliseconds) to pause between LEDs
  for (index = 0; index <= 3; index++)
  {
    shiftWrite(index, HIGH);    // Turn a LED on
    shiftWrite(index + 4, HIGH); // Skip four, and turn that LED on
    delay(delayTime);   // Pause to slow down the sequence
    shiftWrite(index, LOW); // Turn both LEDs off
    shiftWrite(index + 4, LOW);
  }
}


void binaryCount()
{
  int delayTime = 1000; // time (milliseconds) to pause between LEDs
  shiftOut(DATAp, CLOCKp, MSBFIRST, data);
  digitalWrite(LATCHp, HIGH);
  digitalWrite(LATCHp, LOW);
  data++;
  delay(delayTime);
}
