#include <Wire.h>

#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
#define SLAVE_ADDRESS 0x04

#define CEL 1
#define FAR 2

static const int ledsTensPins[] = { LED_BUILTIN, 12, 11, 10, 9, 8, 7 };

static const int ledsOnesPins[] = { 6, 5 ,4, 3, 2, 1, 0 };

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
int return_val = 11;
float temp;
int init_flag;
int tens;
int ones;
int decimal;
int ForC;
int cur_pin;

void setup()
{
   // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  init_flag = 1;
  ForC = FAR;
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  for (i = 0; i < 14; i++)
  {
    pinMode(i, OUTPUT);
    delay(200);
    digitalWrite(i, HIGH);
  }
  delay(1500);
  for (i = 0; i < 14; i++)
    digitalWrite(i, LOW);
//  Serial.begin(9600);
}

void loop()
{
  temp = anaTemp();
//  Serial.print("Temp of Arduino UNO: ");
//  Serial.println(GetTemp(), DEC);
  for (i = 0; i < 7; i++)
  {
      if (ledsTens[tens][i] != 14) digitalWrite(ledsTens[tens][i], HIGH);
      else digitalWrite(ledsTensPins[i], LOW);
      if (ledsOnes[ones][i] != 14) digitalWrite(ledsOnes[ones][i], HIGH);
      else digitalWrite(ledsOnesPins[i], LOW);
  }
  delay(1000);
}


void receiveData(int byteCount)
{
  while (Wire.available())
  {
    return_val = Wire.read();
    switch (return_val)
    {
        case 1:
          return_val = decimal;
          break;
        case 2:
          ForC = (ForC == CEL ? FAR : CEL);
          return_val = 70;
          break;
        case 3: // internal temp of Arduino
          return_val = (int)GetTemp();
          break;
        default:
          break;
    }
  }
    /*
    switch (return_val)
    {
      case 1:
        if (state == 0)
        {
          digitalWrite(13, HIGH);
          state = 1;
        }
        else
        {
          digitalWrite(13, LOW);
          state = 0;
        }
        break;
      case 2:
        return_val = (int)temp;
        break;
      case 3:
        return_val = (int)ldr;
        break;
      case 4:
        finalTemp = tempF * 1000;
        return_val = (int)finalTemp;
          break;
      default:
        break;
    }
  }
  */
}

void sendData()
{
  if (init_flag == 1)
  {
    init_flag = 2;
    return_val = 42;
  }
  Wire.write(return_val);
}

// Get the internal temperature of the Arduino
double GetTemp(void)
{
  unsigned int wADC;
  double t;
  double u;
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN); // enable the ADC
  delay(20); // wait for voltages to become stable
  ADCSRA |= _BV(ADSC); // start the ADC
  while (bit_is_set(ADCSRA, ADSC));
  wADC = ADCW;
  t = (wADC - 324.31) / 1.22;
  u = ((t * 9) / 5) + 32; // convert celcius to farenhiegfhglhethwxyz
  t = u;
  return (t);
}

float anaTemp(void)
{
  long a = analogRead(A0);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = ((tempC * 9) / 5) + 32;
  if (ForC == FAR)
  {
    tens = (int)tempF / 10;
    ones = (int)tempF % 10;
    decimal = 11;
  }
  // For celcius (to be sent via I2C to the Pi
//#ifdef USEI2C
  if (ForC == CEL)
  {
    tempC *= 10;
    decimal = (int)tempC % 10;
    tens = (int)tempC / 100;
    //tenB /= 10;
    ones = (int)tempC % 100;
    ones /= 10;
  }
//#endif
  return tempC;
}

// UNUSED CODE / CODE I DON'T WANT TO LOOSE

    /*
    Serial.print("digitalWrite(");
    Serial.print(ledsTens[tens][i] != 14 ? cur_pin : i);
    Serial.print(", ");
    Serial.print(ledsTens[tens][i] != 14 ? "HIGH" : "LOW");
    Serial.println(");");
    cur_pin = ledsTens[tens][i];
    if (cur_pin != 14)
    {
      Serial.print("digitalWrite(");
      Serial.print(cur_pin);
      Serial.println(", HIGH);");
      //Serial.print(cur_pin);
      //Serial.print(" ");
    }
    else
    {
      Serial.print("digitalWrite(");
      Serial.print(i);
      Serial.println(", LOW);");
    }
  }
  Serial.println("---");
  for (i = 0; i < 7; i++)
  {
    Serial.print("digitalWrite(");
    Serial.print(ledsOnes[ones][i] != 14 ? cur_pin : i);
    Serial.print(", ");
    Serial.print(ledsOnes[ones][i] != 14 ? "HIGH" : "LOW");
    Serial.println(");");
    cur_pin = ledsOnes[ones][i];
    if (cur_pin != 14)
    {
      Serial.print("digitalWrite(");
      Serial.print(cur_pin);
      Serial.println(", HIGH);");
      //Serial.print(cur_pin);
      //Serial.print(" ");
    }
    else
    {
      Serial.print("digitalWrite(");
      Serial.print(i);
      Serial.println(", LOW);");
    }
  }
  Serial.println("===");
  switch (tens)
  {
      case 6:
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsTens[tens][i], (ledsTens[tens][i] != 14 ? HIGH : LOW));
        break;
      case 7:
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsTens[tens][i], (ledsTens[tens][i] != 14 ? HIGH : LOW));
        break;
      case 8:
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsTens[tens][i], (ledsTens[tens][i] != 14 ? HIGH : LOW));
        break;
      case 9:
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsTens[tens][i], (ledsTens[tens][i] != 14 ? HIGH : LOW));
        break;
      default: 
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        break;
  }
  switch (ones)
  {
      case 0:
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 1:
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 2:
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 3:
        for (i = 0; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      case 4:
        for (i = 0; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      case 5:
        for (i = 0; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      case 6:
        for (i = 0; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      case 7:
        for (i = 0; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      case 8:
        for (i = 7; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      case 9:
        for (i = 7; i < 7; i++) digitalWrite(i, LOW);
        for (i = 0; i < 7; i++) digitalWrite(ledsOnes[ones][i], (ledsOnes[ones][i] != 14 ? HIGH : LOW));
        break;
      default:
        break;
  }
  */

