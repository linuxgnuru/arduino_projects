/*
static const int leds[10][7] = {
				{ 10, 9, 8, 0, 6, 5, 4 }, // 0
				{  0, 0, 8, 0, 0, 5, 0 }, // 1
				{ 10, 0, 8, 7, 6, 0, 4 }, // 2
				{ 10, 0, 8, 7, 0, 5, 4 }, // 3
				{  0, 9, 8, 7, 0, 5, 0 }, // 4
				{ 10, 9, 0, 7, 0, 5, 4 }, // 5
				{ 10, 9, 0, 7, 6, 5, 4 }, // 6
				{ 10, 0, 8, 0, 0, 5, 0 }, // 7
				{ 10, 9, 8, 7, 6, 5, 4 }, // 8
				{ 10, 9, 8, 7, 0, 5, 4 }  // 9
				};
*/
static const int ledsTens[10][7] = {
				{ 13, 12, 11, 14,  9,  8,  7 }, // 0
				{ 14, 14, 11, 14, 14,  8, 14 }, // 1
				{ 13, 14, 11, 10,  9, 14,  7 }, // 2
				{ 13, 14, 11, 10, 14,  8,  7 }, // 3
				{ 14, 12, 11, 10, 14,  8, 14 }, // 4
				{ 13, 12, 14, 10, 14,  8,  7 }, // 5
				{ 13, 12, 14, 10,  9,  8,  7 }, // 6
				{ 13, 14, 11, 14, 14,  8, 14 }, // 7
				{ 13, 12, 11, 10,  9,  8,  7 }, // 8
				{ 13, 12, 11, 10, 14,  8,  7 }  // 9
				};

static const int ledsOnes[10][7] = {
				{  6,  5,  4, 14,  2,  1,  0 }, // 0
				{ 14, 14,  4, 14, 14,  1, 14 }, // 1
				{  6, 14,  4,  3,  2, 14,  0 }, // 2
				{  6, 14,  4,  3, 14,  1,  0 }, // 3
				{ 14,  5,  4,  3, 14,  1, 14 }, // 4
				{  6,  5, 14,  3, 14,  1,  0 }, // 5
				{  6,  5, 14,  3,  2,  1,  0 }, // 6
				{  6, 14,  4, 14, 14,  1, 14 }, // 7
				{  6,  5,  4,  3,  2,  1,  0 }, // 8
				{  6,  5,  4,  3, 14,  1,  0 }  // 9
				};

int i;
int j;
int k;
int l;
int m;
int tmp;

void setup()
{
  for (i = 0; i < 14; i++) pinMode(i, OUTPUT);
  for (i = 0; i < 7; i++)
  {
    delay(100);
    digitalWrite(i, HIGH);
  }
//  delay(1500);
  ledCls(0, 7);  
  delay(500);
  for (i = 7; i < 14; i++)
  {
    delay(100);
    digitalWrite(i, HIGH);
  }
  delay(1500);
  ledCls(7, 14);
}

void loop()
{
  /*
  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 7; j++)
    {
        digitalWrite(ledsTens[i][j], (ledsTens[i][j] != 14 ? HIGH : LOW));
    }
    delay(500);
    ledCls(
  }
  */
  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 7; j++)
    {
        digitalWrite(ledsTens[i][j], (ledsTens[i][j] != 14 ? HIGH : LOW));
    }
    for (k = 0; k < 10; k++)
    {
      for (l = 0; l < 7; l++)
      {
        digitalWrite(ledsOnes[k][l], (ledsOnes[k][l] != 14 ? HIGH : LOW));
      }
      delay(1000);
      ledCls(0, 7);
    }
    ledCls(7, 14);
//    ledCls(0, 7);
  }
}

void ledCls(int start, int limit)
{
  for (m = start; m < limit; m++)
  {
    digitalWrite(m, LOW);
  }
}
