#define	TRUE (1==1)
#define	FALSE (!TRUE)

const int hbridgePin = 3;
const int leftPin = 7;
const int rightPin = 8;
const int relayPin = 9;
const int bothPin = 10;
//const int powerPin = 9;
//int flag = FALSE;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(hbridgePin, OUTPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
//  pinMode(powerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(bothPin, OUTPUT);
}

void loop()
{
/*
  flag = (digitalRead(leftPin) == LOW && digitalRead(rightPin) == LOW ? TRUE : FALSE);
  digitalWrite(powerPin, (flag == TRUE ? LOW : HIGH));
  */
//  digitalWrite(powerPin, (digitalRead(leftPin) == LOW && digitalRead(rightPin) == LOW ? LOW : HIGH));
  digitalWrite(relayPin, (digitalRead(leftPin) == LOW && digitalRead(rightPin) == LOW ? LOW : HIGH));
  if (digitalRead(leftPin) == HIGH && digitalRead(rightPin) == HIGH )
  {
    digitalWrite(relayPin, LOW);
    digitalWrite(bothPin, HIGH);
  }
  else
    digitalWrite(bothPin, LOW);
  if (digitalRead(leftPin) == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(hbridgePin, HIGH);
//    analogWrite(hbridgePin, 0);//(map(analogRead(potPin), 0, 1023, 0, 255)));
  }
  if (digitalRead(rightPin) == HIGH)
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(hbridgePin, LOW);
//    analogWrite(hbridgePin, 1023);//(map(analogRead(potPin), 0, 1023, 0, 255)));
  }
  delay(250);
}
