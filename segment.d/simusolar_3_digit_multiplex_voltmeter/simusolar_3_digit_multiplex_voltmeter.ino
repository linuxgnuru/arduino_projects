#define NUM_SAMPLES 10

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

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
int del = 55;  //fine-tuning value for clock

// set the cathode interface
const int d[] = { 2, 3, 4 };

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
  int tens, ones, tmp, decpA, decpB;
  float myvin = getVolt(A2);
  tens = (int)myvin / 10;
  ones = (int)myvin % 10;
  tmp = myvin * 10;
  decpA = (int)tmp % 10;
  /*
    decpB = (int)tmp % 100;
    if (decpB > 10)
    decpB /= 10;
  */
  clearLEDs();
  pickDigit(0);
  pickNumber(tens);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(1);
  digitalWrite(dpins[7], HIGH); // decimal point
  pickNumber(ones);
  delayMicroseconds(del);
  clearLEDs();
  pickDigit(2);
  pickNumber(decpA);
  delayMicroseconds(del);
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

float getVolt(int aPin)
{
  float retVal = 0.0;
  // take a number of analog samples and add them up
  while (sample_count < NUM_SAMPLES)
  {
    sum += analogRead(aPin);
    sample_count++;
    //delay(10);
    delayMicroseconds(10);
  }
  // calculate the voltage
  // use 5.0 for a 5.0V ADC reference voltage
  // 5.015V is the calibrated reference voltage
  voltage = ((float)sum / (float)NUM_SAMPLES * 5.03) / 1024.0;
  //voltage = ((float)sum / (float)NUM_SAMPLES * readVcc()) / 1024.0;
  // send voltage for display on Serial Monitor
  // voltage multiplied by 11 when using voltage divider that
  // divides by 11. 11.132 is the calibrated voltage divide
  // value
  retVal = voltage * 11.069;
  if (retVal < 0.09)
    retVal = 0.0;
  sample_count = 0;
  sum = 0;
  return retVal;
}

