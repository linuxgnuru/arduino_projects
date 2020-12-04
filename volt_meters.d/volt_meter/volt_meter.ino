/*
  /*
  LiquidCrystal Library - Hello World
   LCD RS pin to digital pin 12

   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 7
   LCD D5 pin to digital pin 6
   LCD D6 pin to digital pin 5
   LCD D7 pin to digital pin 4
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

   analog part:
    3.3-30V in ---> 100K ohm /|\   ------> 10K ohm ---> Gnd
                              |
                              |
                           Analog 0
*/

//#include <LiquidCrystal.h>

//LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

//const int anaPin = A0;
const int voltPin = A1;
//const int backlightPin = 3;
//const int alarmPin = 2;

//const float minVolt = 7;

void setup()
{
 // pinMode(backlightPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(alarmPin, OUTPUT);
  //lcd.begin(16, 2);
  Serial.begin(9600);
  //analogWrite(backlightPin, 175);
  //  lcd.setCursor(0, 1);
  //         0123456789012345
  //  lcd.print("jcwiggi@gmail");
  //lcd.setCursor(0, 0);
  //         0123456789012345
  //lcd.print("Volts: ");
}

//boolean flag = false;

void loop()
{
  float vin = 0.0;
#if 0
  if (!checkMyVolt())
  {
    if (!flag)
    {
      lcd.clear();
      flag = true;
    }
    lcd.setCursor(0, 0);
    lcd.print("Sys volt low");
  }
  else
  {
    flag = false;
#endif
  //  vin = getVolt(anaPin);
//    Serial.println(vin);
    /*
    lcd.setCursor(0, 0);
    lcd.print("Volts: ");
    lcd.print(vin, 4);
    //float tmp = readVcc();
*/
    vin = getVolt(voltPin);
    Serial.println(vin);
    /*
    lcd.setCursor(0, 1);
    lcd.print("Input: ");
    lcd.print(vin, 4);
*/
#if 0
  }
#endif
  //Serial.println(vin);
  delay(1000);
}


float getVolt(int aPin)
{
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 100000.0;
  float R2 = 10000.0;
  int value;

  value = analogRead(aPin);
  vout = (value * 5.0) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.09)
    vin = 0.0;
  return vin;
}
/*
boolean checkMyVolt()
{
  float myvin = 0.0;

  myvin = getVolt(voltPin);
  lcd.setCursor(0, 1);
  lcd.print("Input: ");
  lcd.print(myvin, 4);
  return (myvin >= minVolt ? true : false); //|| (myvin > 4.0 && myvin < 5.0)  ? true : false);
}
*/
float readVcc()
{
  float t;
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
  long result = (high<<8) | low;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  t = (float)result / 1000;
  return t; // Vcc in millivolts
}
