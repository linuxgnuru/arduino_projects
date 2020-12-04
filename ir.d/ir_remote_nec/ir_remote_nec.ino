/*
 * IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
 * An IR LED must be connected to Arduino PWM pin 3.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */
#include <IRremote.h>
 
#define NECPower 0x20DF10EF  // Panasonic Power button
#define NECInput 0x20DFD02F
#define NECUp 0x20DF40BF
#define NECDn 0x20DFC03F
#define NECMute 0x20DF906F
#define NECOK 0x20DF22DD
#define NECUp 0x20DF02FD
#define NECDn 0x20DF827D

const int pinPower = 8;
const int pinInput = 9;
const int pinUp = 10;
const int pinDn = 11;
const int pinOK = 12;

int buttonStatePower;
int buttonStateInput;
int buttonStateUp;
int buttonStateDn;
int buttonStateOK;

int lastButtonStatePower = LOW;
int lastButtonStateInput = LOW;
int lastButtonStateUp = LOW;
int lastButtonStateDn = LOW;
int lastButtonStateOK = LOW;

long lastDebounceTimePower = 0;
long lastDebounceTimeInput = 0;
long lastDebounceTimeUp = 0;
long lastDebounceTimeDn = 0;
long lastDebounceTimeOK = 0;

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
    irsend.sendNEC(NECPower, 32);
    blinker();
  }
  lastButtonStatePower = reading;
  reading = digitalRead(pinInput);
  if (reading != lastButtonStateInput) lastDebounceTimeInput = millis();
  if ((millis() - lastDebounceTimeInput) > debounceDelay) buttonStateInput = reading;
  if (buttonStateInput == HIGH)
  {
    irsend.sendNEC(NECInput, 32);
    blinker();
  }
  lastButtonStateInput = reading;
  reading = digitalRead(pinUp);
  if (reading != lastButtonStateUp) lastDebounceTimeUp = millis();
  if ((millis() - lastDebounceTimeUp) > debounceDelay) buttonStateUp = reading;
  else if (buttonStateUp == HIGH)
  {
    irsend.sendNEC(NECUp, 32);
    blinker();
  }
  lastButtonStateUp = reading;
  reading = digitalRead(pinDn);
  if (reading != lastButtonStateDn) lastDebounceTimeDn = millis();
  if ((millis() - lastDebounceTimeDn) > debounceDelay) buttonStateDn = reading;
  else if (buttonStateDn == HIGH)
  {
    irsend.sendNEC(NECDn, 32);
    blinker();
  }
  lastButtonStateDn = reading;
  reading = digitalRead(pinOK);
  if (reading != lastButtonStateOK) lastDebounceTimeOK = millis();
  if ((millis() - lastDebounceTimeOK) > debounceDelay) buttonStateOK = reading;
  else if (buttonStateOK == HIGH)
  {
    irsend.sendNEC(NECOK, 32);
    blinker();
  }
  lastButtonStateOK = reading;
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

/*
int button(int pin, int 
  reading = digitalRead(pinPower);
  if (reading != lastButtonStatePower) lastDebounceTimePower = millis();
  if ((millis() - lastDebounceTimePower) > debounceDelay) buttonStatePower = reading;
  if (buttonStatePower == HIGH)
  {
    irsend.sendNEC(NECPower, 32);
    blinker();
  }
  lastButtonStatePower = reading;
*/
