#include <SPI.h>

#define DEBUG

#include <DebugUtils.h>

byte data[] = {
  0b00000000,
  0b00000000
};

const byte LATCH = 10;
const byte ana_pin = A0;

unsigned int lastMillis = 0;

void setup()
{
  pinMode(LATCH, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
}

void loop()
{
  unsigned int currMillis = millis();
  if (currMillis - lastMillis >= 1000)
  {
    lastMillis = currMillis;
    int ana = abs(analogRead(ana_pin) - 1023);
    Serial.print(F("ana: "));
    Serial.println(ana);
    if (ana < 511)
    {
      data[0] = 0b00000000;
      data[1] = map(ana, 0, 255, 0, 511);
      Serial.print(F(" data[0]"));
      Serial.print(data[1], DEC);
      Serial.print(F(" "));
      Serial.println(data[1], BIN);
    }
    else
    {
      data[1] = 0b11111111;
      int t = ana - 511;
      data[0] = map(t, 0, 255, 0, 511);
      DEBUG_PRINTF("t", t);
      Serial.print(F(" data[0]"));
      Serial.print(data[0], DEC);
      Serial.print(F(" "));
      Serial.println(data[0], BIN);
    }
    wData();
  }
#if 0
  DEBUG_PRINTFLN("val", ana);
}
#endif
#if 0
while (Serial.available() > 0)
{
  int myA = Serial.parseInt();
  int myB = Serial.parseInt();
  //long myA = Serial.read() - 48;
  if (Serial.read() == '\n')
  {
    int mA = B2I(myA);
    int mB = B2I(myB);
    Serial.print(F("myA: "));
    Serial.print(myA);
    Serial.print(F(" myB: "));
    Serial.println(myB);
    Serial.print(F("mA: "));
    Serial.print(mA, BIN);
    Serial.print(F(" mB: "));
    Serial.println(mB, BIN);
    if (mA < 255 && mB < 255)
    {
      data[0] = mA;
      data[1] = mB;
      wData();
    }
  }
  //int val = Serial.read() - 48;
  //Serial.println(val);
  //printDigit(0, val, false);
}
#endif
}

void wData()
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(data[1]);
  SPI.transfer(data[0]);
  digitalWrite(LATCH, HIGH);
}

int B2I(int n)
{
  int decimalNumber = 0;
  int i = 0;
  int remainder;
  while (n != 0)
  {
    remainder = n % 10;
    n /= 10;
    decimalNumber += remainder * pow(2, i);
    ++i;
  }
  return decimalNumber;
}

