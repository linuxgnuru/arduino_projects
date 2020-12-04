// temperature data on Analog 5
// LED display on pins 1-10
int i = 0;
int degree = 25;
int nothing = 100;
float tempC;
float tempF;
float voltage;
int reading;
int num = 11;
int digitOne;
int digitTwo;

unsigned long now;
unsigned long then;

void setup()
{
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  // Using Serial.begin(9600) makes pin 1 stay HIGH for some reason
}


void loop()
{
  now = millis();
  if (now - then > 30000 || millis() < 100) { // take a sample every 30 seconds
    reading = analogRead(5);
    voltage = reading * (5.0 / 1024); //convert reading to voltage (in V), for 5V input
    tempC = (voltage - 0.5) * 100; //convert voltage to temperature
    tempF = ((tempC * 9 / 5) + 32); //convert C temperature to F
    num = tempF; // num takes off the decimal point
    then = now;
  }

  digitOne = num / 10;        //so, to get digitOne you divide by 10
  // a number like 49 magically becomes 4, all decimals disappear
  digitTwo = num - (digitOne * 10); //then to get digit2 (9)you multiply digitOne by 10 to get 40
  // and subtract it from the original 49 to get 9

  //NUMERAL PINS ARE 4, 5, and 9
  //   4 LOW = FIRST NUMERAL
  //   5 LOW = SECOND NUMERAL
  //   9 LOW = THIRD NUMERAL

  //FIRST NUMERAL ON THE DISPLAY

  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  digitalWrite(9, HIGH);

  letterfunction (digitOne); // type 0-9 in the parenthesis or
  //type (degree) for the degree symbol
  // or type (nothing) for nothing

  //NUMERAL 2

  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(9, HIGH);

  letterfunction(digitTwo);


  //NUMERAL 3

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(9, LOW);

  letterfunction(degree);

}
// THIS FUNCTION HAS THE DIFFERENT NUMBERS
// AND THE DEGREE SYMBOL
void letterfunction(unsigned char var) {
  if (var == 1) {
    digitalWrite(6, HIGH); //pins 6 and 2 are the leftmost vertical lines
    digitalWrite(2, HIGH);
    digitalWrite(1, LOW);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 2) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(10, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 3) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(10, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 4) {
    digitalWrite(6, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(1, HIGH);

    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 5) {
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(10, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 6) {
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(10, HIGH);
    delay(5);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    digitalWrite(1, LOW);
    digitalWrite(10, LOW);

  }

  else if (var == 7) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(1, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 8) {
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  } else if (var == 9) {
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(1, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == 0) {
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(1, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(6, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

  else if (var == degree) { //the degree symbol
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    delay(5);
    for (i = 1; i < 11; i++) {
      digitalWrite(i, LOW);
    }
  }

}

