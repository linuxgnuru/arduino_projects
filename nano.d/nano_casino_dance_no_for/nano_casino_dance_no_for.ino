#define MAX_R 3 // seconds

unsigned long previousMillis = 0;

int buttonState[3];
int lastButtonState[3];
long lastDebounceTime[3];
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

const int compTurnPin = A4;
const int humnTurnPin = A1;

const int ledPins[] = {
  LED_BUILTIN, 11, 9, 7, 5, 3, 8, 10, 2, 19, 16, 17
};

const int gpin = 15;
const int bpin = 18;

const byte buttonPins[] = {
  4, 6, 12
};

void setup()
{
  for (int i = 0; i < 3; i++)
  {
    lastButtonState[i] = HIGH;
    lastDebounceTime[i] = 0;
  }
  for (int i = 0; i < 12; i++)
    pinMode(ledPins[i], OUTPUT);
  for (int i = 0; i < 3; i++)
    pinMode(buttonPins[i], INPUT_PULLUP);
}

void loop()
{
  dance();  
}

void dance()
{
  
  for (int i = 12; i > -1; i--)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(30);
    digitalWrite(ledPins[i], LOW);
    delay(30);
  }
  for (int i = 0; i < 12; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(30);
    digitalWrite(ledPins[i], LOW);
    delay(30);
  }
  fauxSleep();
}

void fauxSleep()
{
  unsigned long myTimer;
  int r = 0;

  r = random(1, MAX_R) * 500;
  myTimer = millis();
  while (millis() - myTimer <= r)//15000) // 15000 == 15 seconds
  {
    //  Serial.print("r: ");  Serial.print(r); Serial.print(" myTimer: "); Serial.println(millis() - myTimer);
    // "timer expired"
    if ((millis() - myTimer) % 100 == 0)
    {
      digitalWrite(compTurnPin, LOW);
      delay(150);
      digitalWrite(compTurnPin, HIGH);
      delay(150);
    }
  }
}


  /*
  unsigned long currentMillis = millis();

  int reading;
  for (int i = 0; i < 3; i++)
  {
    reading = digitalRead(buttonPins[i]);
    if (reading != lastButtonState[i])
    {
      lastDebounceTime[i] = millis();
      if ((millis() - lastDebounceTime[i]) > debounceDelay)
      {
        if (reading != buttonState[i])
        {
          buttonState[i] = reading;
          if (buttonState[i] == LOW)
          {
            switch (buttonPins[i])
            {
              case 4:
                digitalWrite(bpin, !digitalRead(bpin));
                break;
              case 6:
                digitalWrite(gpin, !digitalRead(gpin));
                break;
              case 12:
                digitalWrite(gpin, !digitalRead(gpin));
                digitalWrite(bpin, !digitalRead(bpin));
                break;
            }
          }
        }
      }
    }
  }
  #if 0
  for (int i = 0; i < 12; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    //delay(50);
    if (currentMillis - pMa >= 50)
    {
      pMa = currentMillis;
      digitalWrite(ledPins[i], LOW);
    }
  }
  for (int i = 11; i > -1; i--)
  {
    digitalWrite(ledPins[i], HIGH);
    //delay(50);
    if (currentMillis - pMb >= 50)
    {
      pMb = currentMillis;
      digitalWrite(ledPins[i], LOW);
    }
  }
  #endif
}
*/
