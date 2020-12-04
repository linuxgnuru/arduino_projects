#if 0
if ((millis() - prevMillis >= myDelay) && sequenceDone == false)
{
    prevMillis += myDelay;
    if (i < 8)
    {
        chip = 1;
        j = i;
    }
    else
    {
        chip = 0;
        j = i - 8;
    }
    toggle(chip, j);
    i--;
    if (i < 0)
    {
        sequenceDone = true;
    }
}

// Never used:

void pause()
{
  int reading;
  boolean stopFlag = false;

  while (stopFlag == false)
  {
    reading = digitalRead(buttonPin);
    if (reading != lastButtonState)
      lastDebounceTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay)
    {
      if (reading != buttonState)
      {
        buttonState = reading;
        if (buttonState == LOW)
          stopFlag = true;
      }
    }
    lastButtonState = reading;
  }
}

#endif

