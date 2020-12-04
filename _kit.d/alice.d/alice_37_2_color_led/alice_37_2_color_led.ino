// Arduino test code for KY011
int redpin = 11; // select the pin for the red LED - middle pin
int greenpin = 10; // select the pin for the green LED - signal pin
int val;

void setup()
{
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
}

void loop()
{
  digitalWrite(redpin, LOW);
  for (val = 0; val < 255; val++)
  {
    analogWrite(greenpin, val);
    delay(5);
  }
  for (val = 255; val > 0; val--)
  {
    analogWrite(greenpin, val);
    delay(5);
  }
  digitalWrite(greenpin, LOW);
  for (val = 0; val < 255; val++)
  {
    analogWrite(redpin, val);
    delay(5);
  }
  for (val = 255; val > 0; val--)
  {
    analogWrite(redpin, val);
    delay(5);
  }
  for (val = 255; val > 0; val--)
  {
    analogWrite(greenpin, val);
    analogWrite(redpin, 255 - val);
    delay(15);
  }
  for (val = 0; val < 255; val++)
  {
    analogWrite(greenpin, val);
    analogWrite(redpin, 255 - val);
    delay(15);
  }  
}

