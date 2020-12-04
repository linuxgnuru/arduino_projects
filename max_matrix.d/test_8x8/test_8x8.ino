#include <SPI.h>

int LATCH = 10;

byte matrix[8];
int idxa = 0;
int idxb = 0;

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, HIGH);
  Serial.begin(9600);
  initLED();
  clearLED();
}

void loop()
{
  int d = 50;
  for (int j = 0; j < 8; j++)
  {
    for (int i = 1; i < 8; i++)
    {
      updateLED(j, i, true);
      refreshLED();
      delay(d);
    }
    updateLED(j, 0, true);
    refreshLED();
    delay(d);
  }
  for (int j = 0; j < 8; j++)
  {
    for (int i = 1; i < 8; i++)
    {
      updateLED(j, i, false);
      refreshLED();
      delay(d);
    }
    updateLED(j, 0, false);
    refreshLED();
    delay(d);
  }
  /*
    int inp;
      if (Serial.available()) {
      delay(d);
      clearLED();
      while (Serial.available() > 0)
      {
      updateLED(0, atoi(Serial.read()), true);
      refreshLED();
      }
    }
    */
#if 0
  int l;
  for (int j = 0; j < 8; j++)
  {
    updateLED(idxa, j, true);
  }
  refreshLED();
  delay(200);
  for (int j = 0; j < 8; j++)
  {
    updateLED(idxa, j, false);
  }
  refreshLED();
  delay(d);
  for (int j = 0; j < 8; j++)
  {
    updateLED(idxb, j, true);
  }
  refreshLED();
  delay(200);
  for (int j = 0; j < 8; j++)
  {
    updateLED(idxb, j, false);
  }
  refreshLED();
  delay(d);
  idxa++;
  idxa %= 8;
  idxb++;
  idxb %= 8;
#endif
}

void ledOut(int n)
{
  digitalWrite(LATCH, LOW);
  SPI.transfer(n >> 8);
  SPI.transfer(n);
  digitalWrite(LATCH, HIGH);
  delay(1);
  digitalWrite(LATCH, LOW);
}

void initLED()
{
  ledOut(0x0B07);
  ledOut(0x0A0C);
  ledOut(0x0900);
  ledOut(0x0C01);
}

void clearLED()
{
  for (int i = 0; i < 8; i++)
    matrix[i] = 0x00;
  refreshLED();
}

void refreshLED()
{
  int n1, n2, n3;
  for (int i = 0; i < 8; i++)
  {
    n1 = i + 1;
    n2 = matrix[i];
    n3 = (n1 << 8) + n2;
    ledOut(n3);
  }
}

void updateLED(int i, int j, boolean b)
{
  int t = 1;
  int n = 0;
  int m = 0;
  if (j == 0)
    m = 7;
  else
    m = j - 1;
  n = t << m;
  if (b)
    matrix[i] = n | matrix[i];
  else
  {
    n = ~n;
    matrix[i] = n & matrix[i];
  }
}

