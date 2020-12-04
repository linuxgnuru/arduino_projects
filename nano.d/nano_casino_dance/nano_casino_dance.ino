unsigned long previousMillis = 0;
unsigned long pMa = 0;
unsigned long pMb = 0;

int buttonState[3];
int lastButtonState[3];
long lastDebounceTime[3];
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

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
}

