#define	TRUE (1==1)
#define	FALSE (!TRUE)

#define BIGB 1

const int hbPin = 3;
const int leftPin = 7;
const int rightPin = 8;
const int relayPin = 9;
const int bothPin = 10;
#ifdef BIGB
const int offPin = 2;
const int hbPin2 = 5;
const int bothPin2 = 11;
const int relayPin2 = 12;
#else
const int offPin = LED_BUILTIN;
#endif

int leftState;
int rightState;

void setup()
{
  pinMode(offPin, OUTPUT);
  pinMode(hbPin, OUTPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(bothPin, OUTPUT);
#ifdef BIGB
  pinMode(hbPin2, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(bothPin2, OUTPUT);
#endif
}

void loop()
{
  leftState = digitalRead(leftPin);
  rightState = digitalRead(rightPin);
  //digitalWrite(powerPin, (digitalRead(leftPin) == LOW && digitalRead(rightPin) == LOW ? LOW : HIGH)); // test
  // board right next to
  digitalWrite(relayPin, (leftState == LOW && rightState == LOW ? LOW : HIGH));
  // off pin (blue)
  digitalWrite(offPin, (leftState == LOW && rightState == LOW ? HIGH : LOW));
#ifdef BIGB
  // long board
  digitalWrite(relayPin2, (leftState == LOW && rightState == LOW ? LOW : HIGH));
#endif
  if (leftState == HIGH && rightState == HIGH)
  {
    digitalWrite(relayPin, LOW);
    digitalWrite(bothPin, HIGH);
#ifdef BIGB
    digitalWrite(relayPin2, LOW);
    digitalWrite(bothPin2, HIGH);
  }
  else
  {
    digitalWrite(bothPin, LOW);
    digitalWrite(bothPin2, LOW);
  }
#else
  }
  else
    digitalWrite(bothPin, LOW);
#endif
  if ((leftState == LOW && rightState == HIGH) || (leftState == HIGH && rightState == LOW))
  {
    if (leftState == HIGH)
    {
      //digitalWrite(13, HIGH);
      digitalWrite(hbPin, HIGH);
      //analogWrite(hbridgePin, 0);//(map(analogRead(potPin), 0, 1023, 0, 255)));
#ifdef BIGB
      digitalWrite(hbPin2, HIGH);
#endif
    }
    if (rightState == HIGH)
    {
      //digitalWrite(13, LOW);
      digitalWrite(hbPin, LOW);
      //analogWrite(hbridgePin, 1023);//(map(analogRead(potPin), 0, 1023, 0, 255)));
#ifdef BIGB
      digitalWrite(hbPin2, LOW);
#endif
    }
  }
  delay(250);
}

