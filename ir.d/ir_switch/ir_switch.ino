
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;
const int buttonPin = 8;

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop()
{
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState)
    lastDebounceTime = millis();

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        digitalWrite(7, HIGH);
        digitalWrite(6, LOW);
      }
      else
      {
        digitalWrite(7, LOW);
        digitalWrite(6, HIGH);
      }
    }
  }
  lastButtonState = reading;
}
