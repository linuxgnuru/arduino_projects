//set the anode interface
const int a = 1;
const int b = 2;
const int c = 3;
const int d = 4;
const int e = 5;
const int f = 6;
const int g = 7;
const int p = 8;

int pins[8];

// set the cathode interface
const int d4 = 9;
const int d3 = 10;
const int d2 = 11;
const int d1 = 12;

//set variable
long n = 0;
int x = 100;
int del = 55;  //fine-tuning value for clock

const boolean ledNum[][7] = {
  //  a  b  c  d  e  f  g
  { 1, 1, 1, 1, 1, 1, 0 }, // 0
  { 0, 1, 1, 0, 0, 0, 0 }, // 1
  { 1, 1, 0, 1, 1, 0, 1 }, // 2
  { 1, 1, 1, 1, 0, 0, 1 }, // 3
  { 0, 1, 1, 0, 0, 1, 1 }, // 4
  { 1, 0, 1, 1, 0, 1, 1 }, // 5
  { 1, 0, 1, 1, 1, 1, 1 }, // 6
  { 1, 1, 1, 0, 0, 0, 0 }, // 7
  { 1, 1, 1, 1, 1, 1, 1 }, // 8
  { 1, 1, 1, 1, 0, 1, 1 }  // 9
};

void setup()
{
  pinMode(d1, OUTPUT);
  pinMode(d2, OUTPUT);
  pinMode(d3, OUTPUT);
  pinMode(d4, OUTPUT);
  for (int i = 0; i < 8; i++)
  {
    pins[i] = i + 1;
    pinMode(i + 1, OUTPUT);
  }
}

void loop()
{
  clearLEDs();
  pickDigit(1);
  pickNumber((n / x / 1000) % 10);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(2);
  pickNumber((n / x / 100) % 10);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(3);
  dispDec(3);
  pickNumber((n / x / 10) % 10);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(4);
  pickNumber(n / x % 10);
  delayMicroseconds(del);
  n++;
  if (digitalRead(LED_BUILTIN) == LOW)
    n = 0;
}

void pickDigit(int x)  //define pickDigit(x),to open dx port
{
  digitalWrite(d1, HIGH);
  digitalWrite(d2, HIGH);
  digitalWrite(d3, HIGH);
  digitalWrite(d4, HIGH);
  switch (x)
  {
    case 1:
      digitalWrite(d1, LOW);
      break;
    case 2:
      digitalWrite(d2, LOW);
      break;
    case 3:
      digitalWrite(d3, LOW);
      break;
    default:
      digitalWrite(d4, LOW);
      break;
  }
}

void pickNumber(int x)   //define pickNumber(x)to display number x
{
  for (int i = 0; i < 7; i++)
    digitalWrite(pins[i], (ledNum[x][i] == 1 ? HIGH : LOW));
}

void dispDec(int x)  //set to start the decimal point
{
  digitalWrite(p, HIGH);
}

void clearLEDs()  //clear contents
{
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(pins[i], LOW);
  }
}

