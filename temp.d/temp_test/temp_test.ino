#define analogPin A0 //the thermistor attach to 
#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor
#define CEL 1
#define FAR 2

int i;

void setup()
{
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
  //read thermistor value 
  long a = analogRead(analogPin);
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = ((tempC * 9) / 5) + 32;
  int first;
  int last;

  first = (int)tempF / 10;
  last = (int)tempF % 10;
//  last = (int)tempF % 100;
//  dec = tempF - (float)first; // for decimal
//dec *= 100;
//  dec *= 10;
//  last = (int)dec;
/*
  Serial.print(first);
  Serial.print(":");
  Serial.println(last);*/
  switch (first)
  {
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
      if (first >= 10)
      {    
        digitalWrite(3, HIGH); // red
        digitalWrite(2, HIGH); // yellow
        digitalWrite(1, LOW); // green
      }
      if (first <= 5)
      {
        digitalWrite(3, LOW); // red
        digitalWrite(2, HIGH); // yellow
        digitalWrite(1, HIGH); // green
      }
       break;
  }
  switch (last)
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
/*
  Serial.print(first);
  Serial.print(".");
  Serial.print(last);
  Serial.println();
  */
  /*
  Serial.print("Temp:  ");
  Serial.print(tempF);
  Serial.print("  F");
  Serial.println();
  */
  delay(500); //wait for 100 milliseconds
}
