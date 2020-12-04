//set the anode interface
const int dpins[] = {
  5, 6, 7, 8, 9, 10, 11, 12
};

const boolean ledNum[][7] = {
  { 1, 1, 1, 1, 1, 1, 0 }, // 0
  { 0, 1, 1, 0, 0, 0, 0 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 1, 1, 1, 1, 0, 0, 1 }, // 3
  { 0, 1, 1, 0, 0, 1, 1 }, // 4 e d a
  { 1, 0, 1, 1, 0, 1, 1 }, // 5 b e
  { 1, 0, 1, 1, 1, 1, 1 }, // 6 b
  { 1, 1, 1, 0, 0, 0, 0 }, // 7
  { 1, 1, 1, 1, 1, 1, 1 }, // 8
  { 1, 1, 1, 1, 0, 1, 1 }  // 9
};

// set the cathode interface
const int d[] = { 2, 3, 4 };

//set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock

void setup()
{
  int i;
  for (i = 0; i < 3; i++)
    pinMode(d[i], OUTPUT);
  for (i = 0; i < 8; i++)
    pinMode(dpins[i], OUTPUT);
}

void loop()
{
  clearLEDs();
  pickDigit(0);
  pickNumber((n / x / 100) % 10);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(1);
  digitalWrite(dpins[7], HIGH); // decimal point
  pickNumber((n / x / 10) % 10);
  delayMicroseconds(del);

  clearLEDs();
  pickDigit(2);
  pickNumber(n / x % 10);
  delayMicroseconds(del);
  n++;
  //if (digitalRead(LED_BUILTIN) == LOW)    n = 0;
}

void pickDigit(int x)
{
  for (int i = 0; i < 3; i++)
    digitalWrite(d[i], HIGH);
  digitalWrite(d[x], LOW);
}

void pickNumber(int x)   //define pickNumber(x)to display number x
{
  for (int i = 0; i < 7; i++)
    digitalWrite(dpins[i], (ledNum[x][i] == 1 ? HIGH : LOW));
}

void clearLEDs()  //clear contents
{
  for (int i = 0; i < 8; i++)
    digitalWrite(dpins[i], LOW);
}

