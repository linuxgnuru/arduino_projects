#include <Wire.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 

#define SLAVE_ADDRESS 0x04

const int servoPin = 9;
int number = 0;
int input = 0;
//int state = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  // attaches the servo on pin 9 to the servo object 
  myservo.attach(servoPin);
  //Serial.begin(9600);
}

void loop()
{
  /*
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180) 
  myservo.write(val);                  // sets the servo position according to the scaled value 
  */
  delay(15);                           // waits for the servo to get there 
}

void receiveData(int byteCount)
{
  while (Wire.available())
  {
    input = Wire.read();
    if (input >= 0 && input <= 179)
    {
      myservo.write(input); // sets the servo position according to the scaled value 
      number = input;
    }
    else
    {
      number = -1;
    }
  }
}
    /*
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
        number = val;
        break;
      case 3:
        number = val;
        break;
      case 4:
 #ifdef DEBUG
          Serial.print("F: ");
          Serial.println(tempF);
          tempF *= 1000;
          Serial.print("F(1000): ");
          Serial.println(tempF);
          number = tempF;
          Serial.print("number: ");
          Serial.println(number);
#endif
          break;
      default:
        break;
    }
  }
}
*/

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


