#include <Keypad.h>
#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 

const int goodledPin = 11;
const int badledPin = 12;
const int servoPin = 10;

const byte ROWS = 4; 
const byte COLS = 4; 

byte rowPins[ROWS] = { 2, 3, 4, 5 }; //connect to row pinouts 
byte colPins[COLS] = { 6, 7, 8, 9 }; //connect to column pinouts

const char pass[5] = { '1', '7', '0', '1', 'D' };

int numTries = 0;
int pNum = 0; // itteration of password (pass[pNum])

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(goodledPin, OUTPUT);
  pinMode(badledPin, OUTPUT);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object 
//  Serial.begin(9600);
  myservo.write(10);
}

void loop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    if (key == '*' || key == '#')
    {
      numTries = 0;
      pNum = 0;
      myservo.write(10);
      digitalWrite(badledPin, LOW);
      digitalWrite(goodledPin, LOW);
    }
  }
  if (numTries >= 3)
  {
    digitalWrite(badledPin, HIGH);
    myservo.write(10);
  }
  else
  {
    if (key != NO_KEY)
    {
      if (key == pass[pNum])
      {
//        Serial.println(key);
        pNum++;
       }
      else
      {
        numTries++;
      }
    }
    if (pNum >= 5 && digitalRead(badledPin) != HIGH)
    {
      digitalWrite(goodledPin, HIGH);
      myservo.write(179);
    }
  }
  if (digitalRead(goodledPin) == LOW && digitalRead(badledPin) == LOW)
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

