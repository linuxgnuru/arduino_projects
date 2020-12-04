// Variables will change:

int buttonState0;
int lastButtonState0 = HIGH;
unsigned long lastDebounceTime0 = 0;

int buttonState1;
int lastButtonState1 = HIGH;
unsigned long lastDebounceTime1 = 0;

int buttonState2;
int lastButtonState2 = HIGH;
unsigned long lastDebounceTime2 = 0;

unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

boolean led_state[12];
int reading0, reading1, reading2;
int ctr = 0;

const int ledPins[] = {
  LED_BUILTIN, 11, 9, 7, 5, 3, 8, 10, 2, A5, A2, A3
};

const int inputPins[] = { 6, 12, 4 };

const int compTurnPin = A4;
const int humnTurnPin = A1;

#define MAX_R 3 // seconds

boolean who_turn = true;
int loopy = 0;

void setup()
{
  randomSeed(analogRead(A7));
  for (int i = 0; i < 12; i++)
    led_state[i] = true;
  for (int i = 0; i < 12; i++)
    pinMode(ledPins[i], OUTPUT);
  for (int i = 0; i < 3; i++)
    pinMode(inputPins[i], INPUT_PULLUP);
  pinMode(compTurnPin, OUTPUT);
  pinMode(humnTurnPin, OUTPUT);
  for (int i = 0; i < 12; i++)
  {
    digitalWrite(ledPins[i], HIGH);
    delay(100);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop()
{
  if (who_turn)
  {
    turn();
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
          led_state[ctr++] = false;
          who_turn = false;
        }
      }
    }
    lastButtonState0 = reading0;

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
          led_state[ctr++] = false;
          led_state[ctr++] = false;
          who_turn = false;
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
          led_state[ctr++] = false;
          led_state[ctr++] = false;
          led_state[ctr++] = false;
          who_turn = false;
        }
      }
    }
    lastButtonState2 = reading2;
    printLed();
  }
  if (!who_turn)
  {
    turn();
    think();
  }
  // indicates end of game
  if (ctr >= 12)
  {
    ctr = 0;
    for (int i = 12; i > -1; i--)
    {
      digitalWrite(ledPins[i], HIGH);
      delay(70);
      digitalWrite(ledPins[i], LOW);
    }
    reset_();
  }
}

void printLed()
{
  for (int i = 0; i < 12; i++)
    digitalWrite(ledPins[i], led_state[i]);
}

void think()
{
  int c = 0;
  int my_c = 0;

  for (int i = 0; i < 12; i++)
  {
    if (!led_state[i])
      c++;
  }
  //Serial.print("c: "); Serial.println(c);
  if (c < 8)
    fauxSleep(); // pretend to be thinking
  my_c = abs(4 - (c % 4));
  for (int i = 0; i < my_c; i++)
    led_state[ctr++] = false;
  printLed();
  who_turn = true;
}

void reset_()
{
  ctr = 0;
  for (int i = 0; i < 12; i++)
    led_state[i] = true;
//  printLed();
  lastButtonState0 = lastButtonState1 = lastButtonState2 = HIGH;
  lastDebounceTime0 = lastDebounceTime1 = lastDebounceTime2 = 0;
  reading0 = reading1 = reading2 = HIGH;
}

void fauxSleep()
{
  unsigned long myTimer;
  unsigned long r = 0;

  r = random(1, MAX_R) * 1000;
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

void turn()
{
  if (who_turn)
  {
    digitalWrite(humnTurnPin, HIGH);
    digitalWrite(compTurnPin, LOW);
  }
  else
  {
    digitalWrite(humnTurnPin, LOW);
    digitalWrite(compTurnPin, HIGH);
  }
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

