/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */
#include <IRremote.h>
 
#define SonyPower 0x540A  // Panasonic Power button
#define SonyFunc 0x4B0B
#define SonyVolUp 0x240A
#define SonyVolDn 0x640A
#define SonyVolMute 0x140A

const int pinPower = 8;
const int pinFunc = 9;
const int pinVolUp = 10;
const int pinVolDn = 11;
const int pinVolMute = 12;

int buttonStatePower;
int buttonStateFunc;
int buttonStateVolUp;
int buttonStateVolDn;
int buttonStateVolMute;

int lastButtonStatePower = LOW;
int lastButtonStateFunc = LOW;
int lastButtonStateVolUp = LOW;
int lastButtonStateVolDn = LOW;
int lastButtonStateVolMute = LOW;

long lastDebounceTimePower = 0;
long lastDebounceTimeFunc = 0;
long lastDebounceTimeVolUp = 0;
long lastDebounceTimeVolDn = 0;
long lastDebounceTimeVolMute = 0;

long debounceDelay = 50;    // the debounce time; increase if the output flickers

IRsend irsend;
int i;
int reading;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  for (i = 8; i < 13; i++)
    pinMode(i, INPUT);
}

void loop()
{
  reading = digitalRead(pinPower);
  if (reading != lastButtonStatePower) lastDebounceTimePower = millis();
  if ((millis() - lastDebounceTimePower) > debounceDelay) buttonStatePower = reading;
  if (buttonStatePower == HIGH)
  {
    irsend.sendSony(SonyPower, 9);
    blinker();
  }
  lastButtonStatePower = reading;
  reading = digitalRead(pinFunc);
  if (reading != lastButtonStateFunc) lastDebounceTimeFunc = millis();
  if ((millis() - lastDebounceTimeFunc) > debounceDelay) buttonStateFunc = reading;
  if (buttonStateFunc == HIGH)
  {
    irsend.sendSony(SonyFunc, 9);
    blinker();
  }
  lastButtonStateFunc = reading;
  reading = digitalRead(pinVolUp);
  if (reading != lastButtonStateVolUp) lastDebounceTimeVolUp = millis();
  if ((millis() - lastDebounceTimeVolUp) > debounceDelay) buttonStateVolUp = reading;
  else if (buttonStateVolUp == HIGH)
  {
    irsend.sendSony(SonyVolUp, 9);
    blinker();
  }
  lastButtonStateVolUp = reading;
  reading = digitalRead(pinVolDn);
  if (reading != lastButtonStateVolDn) lastDebounceTimeVolDn = millis();
  if ((millis() - lastDebounceTimeVolDn) > debounceDelay) buttonStateVolDn = reading;
  else if (buttonStateVolDn == HIGH)
  {
    irsend.sendSony(SonyVolDn, 9);
    blinker();
  }
  lastButtonStateVolDn = reading;
  reading = digitalRead(pinVolMute);
  if (reading != lastButtonStateVolMute) lastDebounceTimeVolMute = millis();
  if ((millis() - lastDebounceTimeVolMute) > debounceDelay) buttonStateVolMute = reading;
  else if (buttonStateVolMute == HIGH)
  {
    irsend.sendSony(SonyVolMute, 9);
    blinker();
  }
  lastButtonStateVolMute = reading;
/*
  delayMicroseconds(50); // see http://www.sbprojects.com/knowledge/ir/jvc.php for information
  irsend.sendJVC(JVCPower, 16,1); // hex value, 16 bits, repeat
  delayMicroseconds(50);
  */
}

void blinker()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
}
