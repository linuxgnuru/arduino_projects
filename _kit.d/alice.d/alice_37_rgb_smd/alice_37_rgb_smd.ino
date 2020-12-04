/*
NOTE:
pin 11 -> 180 ohm resistor -> R
pin 10 -> 100 ohm resistor -> B
pin  9 -> 100 ohm resistor -> G
*/
int rPin = 11; // select the pin for the red LED
int bPin = 10; // select the pin for the blue LED
int gPin =  9; // select the pin for the green LED
int val=0;

void setup ()
{
  pinMode (rPin, OUTPUT);
  pinMode (bPin, OUTPUT);
  pinMode (gPin, OUTPUT);
//  Serial.begin (gPin600);
}
 
void loop ()
{
  for (val=255; val>0; val--)
  {
    analogWrite (rPin, val);
    analogWrite (bPin, 255-val);
    analogWrite (gPin, 128-val);
    delay (5);
  }
/*
    analogWrite (rPin, 0);
    analogWrite (bPin, 0);
    analogWrite (gPin, 0);
  delay(500);
  */
  for (val = 0; val <255; val++)
  {
    analogWrite (rPin, val);
    analogWrite (bPin, 255-val);
    analogWrite (gPin, 128-val);
    delay (5);
  }
//  delay(500);
// Serial.println (val, DEC);
}
