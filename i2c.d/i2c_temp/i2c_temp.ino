#include <Wire.h>

#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor

#define SLAVE_ADDRESS 0x04
int number = 0;
int dec = 0;
int state = 0;
int ldr;
long a;
double temp;

float tempC;
float tempF;
float finalTemp;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
 // Serial.begin(9600);
}

void loop()
{
  delay(100);
  temp = GetTemp();
  finalTemp = anaTemp();
  ldr = anaLDR();
}

void receiveData(int byteCount)
{
  while (Wire.available())
  {
    number = Wire.read();
    switch (number)
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
        number = (int)temp;
        break;
      case 3:
        number = (int)ldr;
        break;
      case 4:
        finalTemp = tempF * 1000;
        number = (int)finalTemp;
      /*
          Serial.print("F: ");
          Serial.println(tempF);
          tempF *= 1000;
          Serial.print("F(1000): ");
          Serial.println(tempF);
          number = tempF;
          Serial.print("number: ");
          Serial.println(number);
          */
          break;
      default:
        break;
    }
  }
}

void sendData()
{
  Wire.write(number);
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
  a = analogRead(A0);
  //the calculating formula of temperature
  tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  tempF = ((tempC * 9) / 5) + 32;
}

double anaLDR(void)
{
  return (analogRead(A1));
}
