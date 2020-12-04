/*
  Design a circuit and write a test program, using the Arduino Nano,
  that will test two input Transistor-Transistor Logic (TTL) gates
  including AND, OR, NAND, and XOR.
  LCD RS pin to digital pin 12
  LCD Enable pin to digital pin 11
  LCD D4 pin to digital pin 5
  LCD D5 pin to digital pin 4
  LCD D6 pin to digital pin 3
  LCD D7 pin to digital pin 2
  LCD R/W pin to ground
  10K potentiometer:
  ends to +5V and ground
  wiper to LCD VO pin (pin 3)
*/
#include <LiquidCrystal.h>
#include "symbols.h"

LiquidCrystal lcd(12, 11, 4, 5, 6, 7);

typedef enum {
  AND_G,
  OR_G,
  NOT_G,
  NAND_G,
  NOR_G,
  XOR_G
} gate_enum;


int gatevalue = 0;

const int inputPin = 8;
const int Output1Pin = 2;
const int Output2Pin = 3;

const int leds[] = {
  A0, A1, A2, A3, A4
};

//Begin declaring what pin is what
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  lcd.begin(16, 2);

  //lcd.createChar(1, output);
  lcd.createChar(2, negate);
  lcd.createChar(3, notTop);
  lcd.createChar(4, orBot);
  lcd.createChar(5, sqBot);
  //  lcd.createChar(5, flatBot);
  lcd.createChar(6, orTop);
  lcd.createChar(0, input);
  lcd.createChar(7, andTop); // 8 -> 7
  lcd.createChar(1, xorBot); // 9 -> 1
  /*
    AND: input sqBot andTop output
     OR: input orBot orTop output
    NOT: input sqBot notTop negate output
    NAND: input sqBot andTop negate output
    NOR: input orBot orTop negate output
    XOR: input xorBot orTop output
  */
  /*
    lcd.createChar(0, ANDg);
    lcd.createChar(1, ORg);
    lcd.createChar(2, NOTg);
    lcd.createChar(3, NANDg);
    lcd.createChar(4, NORg);
    lcd.createChar(5, XORg);
  */
  //Serial.begin(9600);
  pinMode (Output1Pin, OUTPUT);
  pinMode (Output2Pin, OUTPUT);
  pinMode (inputPin, INPUT);
  for (int i = 0; i < 5; i++)
    pinMode(leds[i], OUTPUT);
  gatevalue = 0;
}

void loop()
{
  //When an input is HIGH it will add a value to the gatevalue
  gatevalue = gatevalue + (check_Gate(false, false) * 8);
  //and each gate will have a different gatevalue.
  gatevalue = gatevalue + (check_Gate(false, true) * 4);
  gatevalue = gatevalue + (check_Gate(true, false) * 2);
  gatevalue = gatevalue + (check_Gate(true, true) * 1);

  switch (gatevalue)
  {
    case 1:
      //When gatevalue is 1 it is an AND gate
      lcd.setCursor(0, 1);
      for (int i = 0; i < 16; i++)
        lcd.print(" ");
      lcd.setCursor(0, 0);
      lcd.print("AND gate ");
      printSym(AND_G);
      //if gate is in, light is off
      allOff();
      // AND gate truth table outputs are 0, 0, 0, 1 which is how the LED’s are set up
      digitalWrite(leds[4], HIGH);
      break;
    case 6:
      lcd.setCursor(0, 1);
      for (int i = 0; i < 16; i++)
        lcd.print(" ");
      lcd.setCursor(0, 0);
      lcd.print("XOR gate ");
      printSym(XOR_G);
      //if gate is in, light is off
      allOff();
      //Truth table values
      digitalWrite(leds[2], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    case 7:
      lcd.setCursor(0, 1);
      for (int i = 0; i < 16; i++)
        lcd.print(" ");
      lcd.setCursor(0, 0);
      lcd.print("OR gate ");
      printSym(OR_G);
      allOff();
      //if gate is in, light is off
      //Truth table values
      digitalWrite(leds[2], HIGH);
      digitalWrite(leds[3], HIGH);
      digitalWrite(leds[4], HIGH);
      break;
    case 8:
      lcd.setCursor(0, 1);
      for (int i = 0; i < 16; i++)
        lcd.print(" ");
      lcd.setCursor(0, 0);
      lcd.print("NOR gate ");
      printSym(NOR_G);
      allOff();
      digitalWrite(leds[2], LOW);
      digitalWrite(leds[3], LOW);
      digitalWrite(leds[4], HIGH);
    break;
    case 14:
      lcd.setCursor(0, 1);
      for (int i = 0; i < 16; i++)
        lcd.print(" ");
      lcd.setCursor(0, 0);
      //         01234567890123456
      lcd.print("NAND gate ");
      printSym(NAND_G);
      //if gate is in, light is off
      allOff();
      //Truth table values
      digitalWrite(leds[1], HIGH);
      digitalWrite(leds[2], HIGH);
      digitalWrite(leds[3], HIGH);
      break;
    default:
      lcd.setCursor(0, 0);
      //         01234567890123456
      lcd.print("ERROR: ");
      lcd.print(gatevalue);
      lcd.setCursor(0, 1);
      //         01234567890123456
      lcd.print("Gate Not Present.");
      //if gate is not in, light is on
      allOff();
      digitalWrite(leds[0], HIGH);
  }
  gatevalue = 0;
  delay(1000);
}

int check_Gate(int output1, int output2)
{
  int x;
  digitalWrite(Output1Pin, output1);
  digitalWrite(Output2Pin, output2);
  // Make sure the signal has time to propogate through the gate.
  delay(5);
  x = digitalRead(inputPin);
  return x;
}

void allOff()
{
  for (int i = 0; i < 5; i++)
    digitalWrite(leds[i], LOW);
}

void printSym(int s)
{
  switch (s)
  {
    case AND_G:
      lcd.write(5);
      lcd.write(7);
      lcd.print("-");
      break;
    case OR_G:
      lcd.write(4);
      lcd.write(6);
      lcd.print("-");
      break;
    case NOT_G:
      lcd.write(5);
      lcd.write(3);
      lcd.write(2);
      lcd.print("-");
      break;
    case NAND_G:
      lcd.write(5);
      lcd.write(7);
      lcd.write(2);
      lcd.print("-");
      break;
    case NOR_G:
      lcd.write(4);
      lcd.write(6);
      lcd.write(2);
      lcd.print("-");
      break;
    case XOR_G:
      lcd.write((byte) 1);
      lcd.write(6);
      lcd.print("-");
      break;
    default:
      break;
  }
}

