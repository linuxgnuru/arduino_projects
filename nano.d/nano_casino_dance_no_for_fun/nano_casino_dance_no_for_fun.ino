#define MAX_R 3 // seconds

unsigned long previousMillis = 0;
unsigned long lastMillisBlinkH = 0;
unsigned long lastMillisBlinkC = 0;

unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

const int compTurnPin = A4;
const int humnTurnPin = A1;

boolean last_human = LOW;

const int ledPins[] = {
  LED_BUILTIN, 11, 9, 7, 5, 3, 8, 10, 2, 19, 16, 17
};

const int gpin = 15;
const int bpin = 18;

const int inputPins[] = { 6, 12, 4 };

int reading0, reading1, reading2;

int buttonState0;
int lastButtonState0 = HIGH;
long lastDebounceTime0 = 0;

int buttonState1;
int lastButtonState1 = HIGH;
long lastDebounceTime1 = 0;

int buttonState2;
int lastButtonState2 = HIGH;
long lastDebounceTime2 = 0;

void setup()
{
  for (int i = 0; i < 12; i++)
    pinMode(ledPins[i], OUTPUT);
  for (int i = 0; i < 3; i++)
    pinMode(inputPins[i], INPUT_PULLUP);
  digitalWrite(compTurnPin, HIGH);
}

boolean keep_dancing = false;

void loop()
{
  reading0 = digitalRead(inputPins[0]);
  if (reading0 != lastButtonState0)
    lastDebounceTime0 = millis();
  if ((millis() - lastDebounceTime0) > debounceDelay)
  {
    if (reading0 != buttonState0)
    {
      buttonState0 = reading0;
      if (buttonState0 == LOW)
      {
        keep_dancing = !keep_dancing;
      }
    }
  }
  lastButtonState0 = reading0;
  if (keep_dancing)
    dance();

  reading1 = digitalRead(inputPins[1]);
  if (reading1 != lastButtonState1)
    lastDebounceTime1 = millis();
  if ((millis() - lastDebounceTime1) > debounceDelay)
  {
    if (reading1 != buttonState1)
    {
      buttonState1 = reading1;
      if (buttonState1 == LOW)
      {
        dance();
      }
    }
  }
  lastButtonState1 = reading1;

  reading2 = digitalRead(inputPins[2]);
  if (reading2 != lastButtonState2)
    lastDebounceTime2 = millis();
  if ((millis() - lastDebounceTime2) > debounceDelay)
  {
    if (reading2 != buttonState2)
    {
      buttonState2 = reading2;
      if (buttonState2 == LOW)
      {
        for (int i = 12; i > -1; i--)
          digitalWrite(ledPins[i], LOW);
        digitalWrite(compTurnPin, LOW);
        digitalWrite(humnTurnPin, LOW);
        last_human = HIGH;
      }
    }
  }
  lastButtonState2 = reading2;
  /*
    blink_(100, humnTurnPin);
    blink_(170, compTurnPin);
  */
}

int ctrA = 12;
int ctrB = 0;

void dance()
{
  unsigned int curMillis = millis();
  digitalWrite(compTurnPin, !digitalRead(compTurnPin));
  digitalWrite(humnTurnPin, !last_human);
  last_human = !last_human;
  for (int i = 12; i > -1; i--)
  {
    if (digitalRead(inputPins[0]) == LOW)
      break;
    digitalWrite(ledPins[i], HIGH);
    delay(30);
    digitalWrite(ledPins[i], LOW);
    //delay(30);
  }
  for (int i = 0; i < 12; i++)
  {
    if (digitalRead(inputPins[0]) == LOW)
      break;
    digitalWrite(ledPins[i], HIGH);
    delay(30);
    digitalWrite(ledPins[i], LOW);
    //delay(30);
  }
}

/*
  void fauxSleep()
  {
  unsigned int myTimer;
  unsigned int r = 0;

  r = random(1, MAX_R) * 500;
  myTimer = millis();
  while (millis() - myTimer <= r)//15000) // 15000 == 15 seconds
  {
    //  Serial.print("r: ");  Serial.print(r); Serial.print(" myTimer: "); Serial.println(millis() - myTimer);
    // "timer expired"
    if ((millis() - myTimer) % 100 == 0)
    {
      digitalWrite(compTurnPin, LOW);
      delay(75);
      digitalWrite(compTurnPin, HIGH);
      delay(75);
    }
  }
  }
*/

void blink_(int secs, int pin)
{
  unsigned int currentMillis = millis();
  if (pin == humnTurnPin)
  {
    if (currentMillis - lastMillisBlinkH >= (unsigned int) secs)
    {
      lastMillisBlinkH = currentMillis;
      digitalWrite(pin, !last_human);
      last_human = !last_human;
    }
  }
  else
  {
    if (currentMillis - lastMillisBlinkC >= (unsigned int) secs)
    {
      lastMillisBlinkC = currentMillis;
      digitalWrite(pin, !digitalRead(pin));
    }
  }
}

