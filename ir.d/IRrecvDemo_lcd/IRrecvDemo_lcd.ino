/*
   IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/

#include <IRremote.h>
#include <LiquidCrystal.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;
//                RS E  D4 D5 D6 D7
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  lcd.begin(16, 2);
  //Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  //Serial.println(F(" -- READY --"));
  lcd.setCursor(0, 0);
  lcd.print("Ready");
}

void loop() {
  char buf[9];
  if (irrecv.decode(&results)) {
    sprintf(buf, "%04X", results.value);
    if (results.value < 4294967295)
    {
      if (results.value < 17753245 && results.value > 255)
      {
        //Serial.print(F("D: "));
        //Serial.println(results.value, DEC);
        //Serial.print(F("X: "));
        //Serial.println(results.value, HEX);
        lcd.setCursor(0, 1);
        for (int i = 0; i < 16; i++)
          lcd.print(" ");
        lcd.setCursor(0, 1);
        //Serial.print(F("LCD: "));
        //Serial.println(buf);
        lcd.print(buf);
        //Serial.println();
      }
      else
      {
        lcd.setCursor(0, 0);
        lcd.print("Other: ");
        lcd.print(results.value, HEX);
        //Serial.print(F("other "));
        //Serial.println(results.value, HEX);
#ifdef YES_
        Serial.print(F("=-= OVER -> ["));
        Serial.print(results.value, DEC);
        Serial.print(F("] ["));
        Serial.print(results.value, HEX);
        Serial.println(F("] =-="));
#endif
      }
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}

