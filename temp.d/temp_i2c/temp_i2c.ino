#include <Wire.h>

#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
#define SLAVE_ADDRESS 0x04

#define CEL 1
#define FAR 2

int i;
int return_val = 11;
float temp;
int init_flag;
int tens;
int ones;
int decimal;
int ForC;
int tmp_reading;
int buttonState;
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

void setup()
{
   // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  init_flag = 1;
  ForC = FAR;
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  pinMode(0, INPUT);
  for (i = 1; i < 14; i++)
  {
    pinMode(i, OUTPUT);
    delay(100);
    digitalWrite(i, HIGH);
  }
  delay(1500);
  for (i = 1; i < 14; i++)
  {
    digitalWrite(i, LOW);
  }
//  Serial.begin(9600);
}

void loop()
{
  //delay(100);
  // debouncer
  /*
  tmp_reading = digitalRead(0);
  if (tmp_reading != lastButtonState)
    lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (tmp_reading != buttonState)
    {
      buttonState = tmp_reading;
      if (buttonState == HIGH)
      {
        ForC = (ForC == CEL ? FAR : CEL);
//        Serial.print("change: ");
//        Serial.println(ForC);
      }
    }
  }
  lastButtonState = tmp_reading;
  */
//  ForC = CEL;
  temp = anaTemp();
  switch (tens)
  {
    case 2: // celcius
      digitalWrite(3, LOW); // red
      digitalWrite(2, LOW); // yellow
      digitalWrite(1, HIGH); // green
      break;
    case 3: // celcius
      digitalWrite(3, LOW); // red
      digitalWrite(2, HIGH); // yellow
      digitalWrite(1, LOW); // green
      break;
    case 6: // cold
      digitalWrite(3, HIGH); // red
      digitalWrite(2, LOW); // yellow
      digitalWrite(1, HIGH); // green
      break;
    case 7: // just right
      digitalWrite(3, LOW); // red
      digitalWrite(2, LOW); // yellow
      digitalWrite(1, HIGH); // green
      break;
    case 8: // kinda hot
      digitalWrite(3, LOW); // red
      digitalWrite(2, HIGH); // yellow
      digitalWrite(1, LOW); // green
      break;
    case 9: // pretty hot
        digitalWrite(3, HIGH); // red
        digitalWrite(2, LOW); // yellow
        digitalWrite(1, LOW); // green
      break;
     default: // very hot or very cold
      if (tens >= 10)
      {    
        digitalWrite(3, HIGH); // red
        digitalWrite(2, HIGH); // yellow
        digitalWrite(1, LOW); // green
      }
      if (tens <= 5)
      {
        digitalWrite(3, LOW); // red
        digitalWrite(2, HIGH); // yellow
        digitalWrite(1, HIGH); // green
      }
       break;
  }
  switch (ones)
  {
      case 0:
        digitalWrite(4, HIGH);
        for (i = 5; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 1:
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        for (i = 6; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 2:
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        for (i = 7; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 3:
        for (i = 4; i < 7; i++) digitalWrite(i, LOW);
        digitalWrite(7, HIGH);
        for (i = 8; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 4:
        for (i = 4; i < 8; i++) digitalWrite(i, LOW);
        digitalWrite(8, HIGH);
        for (i = 9; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 5:
        for (i = 4; i < 9; i++) digitalWrite(i, LOW);
        digitalWrite(9, HIGH);
        for (i = 10; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 6:
        for (i = 4; i < 10; i++) digitalWrite(i, LOW);
        digitalWrite(10, HIGH);
        for (i = 11; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 7:
        for (i = 4; i < 11; i++) digitalWrite(i, LOW);
        digitalWrite(11, HIGH);
        for (i = 12; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 8:
        for (i = 4; i < 12; i++) digitalWrite(i, LOW);
        digitalWrite(12, HIGH);
        for (i = 13; i < 14; i++) digitalWrite(i, LOW);
        break;
      case 9:
        for (i = 4; i < 13; i++) digitalWrite(i, LOW);
        digitalWrite(LED_BUILTIN, HIGH);
        break;
  }
//  int tempT = tempF * 1000;
//  Serial.print("TempF: ");
//  Serial.println(tempF);
//  Serial.print("dec:");
//  Serial.println(dec);
  delay(500); //wait for 100 milliseconds
}

void receiveData(int byteCount)
{
  while (Wire.available())
  {
    return_val = Wire.read();
    if (return_val == 1)
      return_val = decimal;
    if (return_val == 2)
      ForC = (ForC == CEL ? FAR : CEL);
  }
    /*
    switch (return_val)
    {
      case 1:
        if (state == 0)
        {
          digitalWrite(LED_BUILTIN, HIGH);
          state = 1;
        }
        else
        {
          digitalWrite(LED_BUILTIN, LOW);
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
  u = ((t * 9) / 5) + 32;
  t = u;
  return (t);
}

float anaTemp(void)
{
  long a = analogRead(A0);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = ((tempC * 9) / 5) + 32;
//  last = (int)tempF % 100;
//  dec = tempF - (float)first; // for decimal
//dec *= 100;
//  dec *= 10;
//  last = (int)dec;
/*
  Serial.print(first);
  Serial.print(":");
  Serial.println(last);*/
  if (ForC == FAR)
  {
    tens = (int)tempF / 10;
    ones = (int)tempF % 10;
    decimal = 11;
  }
  if (ForC == CEL)
  {
    tempC *= 10;
    /*
      test = round(numA);
      printf("test: %l\n", test);
    */
    decimal = (int)tempC % 10;
    tens = (int)tempC / 100;
    //tenB /= 10;
    ones = (int)tempC % 100;
    ones /= 10;
  }
}

