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
  lcd.begin(16, 2);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println(F(" -- READY --"));
  lcd.setCursor(0, 0);
  lcd.print("Ready for remote:");
}

int sz;

void loop() {
  char buf[9];
  if (irrecv.decode(&results)) {
    sprintf(buf, "%04x", results.value);
    sz = strlen(buf);
    //Serial.print(F("[")); Serial.print(sz); Serial.print(F("] "));
    //Serial.print(results.value, HEX);
    //lcd.print(results.value, HEX);
    //if (buf[3] != 0xffff)
    //Serial.print(F(" (")); Serial.print(buf[3]); Serial.print(F(") "));
    Serial.println(F(" - NEW - "));
    for (int i = 0; i < 4; i++)
    {
      Serial.print(F("["));
      Serial.print(buf[i], DEC);
      Serial.print(F("]"));
    }
    Serial.println();
    if (buf[0] != 102 && buf[1] != 102 && buf[2] != 102 && buf[3] != 102)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ready for remote:");
      lcd.setCursor(0, 1);
      //Serial.print(buf);
      lcd.print(buf);
    }
    Serial.println();
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
