// 14 = turn off LED
const int allLed[] = {
 2,  3,  4,  5,  6,  7, 8
};

const int digitPin[][7] = {
  {  2,  3,  4,  5,  6,  7, 14 }, // 0
  { 14,  3,  4, 14, 14, 14, 14 }, // 1
  {  2,  3, 14,  5,  6, 14,  8 }, // 2
  {  2,  3,  4,  5, 14, 14,  8 }, // 3
  { 14,  3,  4, 14, 14,  7,  8 }, // 4
  {  2, 14,  4,  5, 14,  7,  8 }, // 5
  {  2, 14,  4,  5,  6,  7,  8 }, // 6
  {  2,  3,  4, 14, 14, 14, 14 }, // 7
  {  2,  3,  4,  5,  6,  7,  8 }, // 8
  {  2,  3,  4,  5, 14,  7,  8 }  // 9
};

const int dpPin = 9;

const int colPin[] = {  10, 11, 12 };

void setup()
{
  for (int i = 2; i < 14; i++)
    pinMode(i, OUTPUT);
  // turn off all the columns
  for (int i = 0; i < 3; i++)
    digitalWrite(colPin[i], HIGH);
}

void loop()
{
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(colPin[i], LOW);
    for (int j = 0; j < 10; j++)
    {
      printDigit(j, false);
      delay(500);
    }
    digitalWrite(colPin[i], HIGH);
  }
}

void printDigit(int num, boolean dpFlag)
{
  // First, the decimal point
  digitalWrite(dpPin, (dpFlag == true ? HIGH : LOW));
  for (int i = 0; i < 7; i++)
  {
    if (digitPin[num][i] != 14)
      digitalWrite(digitPin[num][i], HIGH);
    else
      digitalWrite(allLed[i], LOW);
  }
}
