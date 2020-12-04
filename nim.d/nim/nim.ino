// Variables will change:

int buttonState0;
int lastButtonState0 = HIGH;
long lastDebounceTime0 = 0;

int buttonState1;
int lastButtonState1 = HIGH;
long lastDebounceTime1 = 0;

int buttonState2;
int lastButtonState2 = HIGH;
long lastDebounceTime2 = 0;

long debounceDelay = 50;    // the debounce time; increase if the output flickers

boolean led_state[12];
int reading;
int ctr = 0;

const int ledPins[] = {
  2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, LED_BUILTIN 
};

const int inputPins[] = { A0, A1, A2 };

const int compTurnPin = A3;
const int humnTurnPin = A4;

#define MAX_R 10 // 10 seconds

boolean who_turn = true;

void setup()
{
  randomSeed(analogRead(A7));
  for (int i = 0; i < 12; i++)
    led_state[i] = true;
  for (int i = 0; i < 12; i++)
  {
    pinMode(lPins[i], OUTPUT);
  }
  pinMode(iPins[0], INPUT_PULLUP);
  pinMode(iPins[1], INPUT_PULLUP);
  pinMode(iPins[2], INPUT_PULLUP);
  //pinMode(A5, OUTPUT);
  //digitalWrite(A5, LOW);
  //Serial.begin(9600);
}

void loop()
{
  if (who_turn)
  {
    reading = digitalRead(iPins[0]);
    if (reading != lastButtonState0)    lastDebounceTime0 = millis();
    if ((millis() - lastDebounceTime0) > debounceDelay)
    {
      if (reading != buttonState0)
      {
        buttonState0 = reading;
        if (buttonState0 == LOW)
        {
          led_state[ctr++] = false;
          who_turn = false;
        }
      }
    }
    lastButtonState0 = reading;
    reading = digitalRead(iPins[1]);
    if (reading != lastButtonState1)    lastDebounceTime1 = millis();
    if ((millis() - lastDebounceTime1) > debounceDelay)
    {
      if (reading != buttonState1)
      {
        buttonState1 = reading;
        if (buttonState1 == LOW)
        {
          led_state[ctr++] = false;
          led_state[ctr++] = false;
          who_turn = false;
        }
      }
    }
    lastButtonState1 = reading;
    reading = digitalRead(iPins[2]);
    if (reading != lastButtonState2)
      lastDebounceTime2 = millis();
    if ((millis() - lastDebounceTime2) > debounceDelay)
    {
      if (reading != buttonState2)
      {
        buttonState2 = reading;
        if (buttonState2 == LOW)
        {
          led_state[ctr++] = false;
          led_state[ctr++] = false;
          led_state[ctr++] = false;
          who_turn = false;
        }
      }
    }
    lastButtonState2 = reading;
    pLed();
  }
  if (!who_turn)
  {
    delay(500);
    think();
  }
  // indicates end of game
  if (ctr >= 12)
  {
    for (int i = 12; i > -1; i--)
    {
      digitalWrite(lPins[i], HIGH);
      delay(30);
      digitalWrite(lPins[i], LOW);
      //delay(20);
    }
    reset_();
  }
}

void pLed()
{
  for (int i = 0; i < 12; i++)
    digitalWrite(lPins[i], (led_state[i] ? HIGH : LOW));
  /*
    for (int i = 0; i < ctr; i++)    digitalWrite(lPins[i], HIGH);
    for (int i = ctr; i < 12; i++)    digitalWrite(lPins[i], LOW);
  */
}

void blip()
{
  digitalWrite(A5, HIGH);
  delay(150);
  digitalWrite(A5, LOW);
}

void pState()
{
  Serial.print("led_state: [");
  for (int i = 0; i < 12; i++)
  {
    Serial.print(led_state[i]);
    if (i != 11)
      Serial.print(" ");
  }
  Serial.println("]");
}

void think()
{
  int c = 0;
  int mc = 0;
  int t_ctr = 0;
  for (int i = 0; i < 12; i++)
  {
    if (!led_state[i])
      c++;
  }
  mc = abs(4 - (c % 4));
  /*
  Serial.print("c: ");
  Serial.print(c);
  Serial.print(" mc: ");
  Serial.print(mc);
  Serial.print(" led_state[mc]: ");
  Serial.println(led_state[mc]);
  Serial.print("pstate 1st: ");
  pState();
  */
  for (int i = 0; i < mc; i++)
    led_state[ctr++] = false;
  //Serial.print("pstate 2nd: ");  pState();
  pLed();
  who_turn = true;
}

void reset_()
{
  ctr = 0;
  for (int i = 0; i < 12; i++)
    led_state[i] = true;
  pLed();
}

void fauxSleep(int dl)
{
  unsigned long myTimer;
  int r = 0;
  r = random(2, MAX_R);
  r *= 1000;
  myTimer = millis();
  while (millis() - myTimer < r)//15000) // 15000 == 15 seconds
  {
    // "timer expired"
    digitalWrite(compTurnPin, HIGH);
    
  }
}

