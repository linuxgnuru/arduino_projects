/*
 * VSS to ground
 * VDD to +3/5V
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5 // 8
 * LCD D5 pin to digital pin 4 // 9
 * LCD D6 pin to digital pin 3 // 10
 * LCD D7 pin to digital pin 2 // 13
 * LCD R/W pin to ground
 * 10K resistor: ends to +3/5V and ground, wiper to LCD VO pin (pin 3)
 * A to +3/5V
 * K to ground
 */
 
#include <Keypad.h>
/*
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 8, 9, 10, 13);

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

const char resetPass[3] = { '*', '#', '*' };
const char easterEgg[4] = { '1', '9', '6', '5' };

int numTries = 0;
int pNum = 0;
int eNum = 0;
static int curCol = 0;
static int curRow = 0;
*/

const byte ROWS = 4; 
const byte COLS = 4; 

//byte rowPins[ROWS] = { 0, 1, 2, 3 }; //connect to row pinouts 
//byte colPins[COLS] = { 4, 5, 6, 7 }; //connect to column pinouts

byte rowPins[ROWS] = { 12, 11, 10, 9 }; //connect to row pinouts 
byte colPins[COLS] = { 8, 7, 6, 5 }; //connect to column pinouts

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  /*
}
  lcd.begin(16, 2);
  lcd.createChar(0, smiley);
  lcd.createChar(1, armsDown);  
  lcd.createChar(2, armsUp);  
  lcd.blink();
  lcd.setCursor(curCol, curRow);
  */
  Serial.begin(9600);
}

void loop()
{
  char key = keypad.getKey();
  if (key != NO_KEY)
  {
    Serial.println(key);
  }
}
    /*
    if (key == easterEgg[eNum])
    {
      eNum++;
    }
    if (eNum >= 4)
    {
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.write(byte(0));
      lcd.setCursor(5, 0);
      lcd.print("Happy");
      lcd.setCursor(11, 0);
      lcd.write(byte(0));
      lcd.setCursor(2, 1);
      //       012345678901234
      lcd.print("Anniversary!");
      for (int z = 0; z < 10; z++)
      {
        lcd.setCursor(0, 1);
        lcd.write(byte(1));
        lcd.setCursor(15, 1);
        lcd.write(byte(1));
        delay(300);
        lcd.setCursor(0, 1);
        lcd.write(byte(2));
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
        delay(300);
      }
      lcd.setCursor(0, 0);
      lcd.blink();
      lcd.clear();
      curCol = curRow = 0;
      eNum = 0;
    }
    if (key == resetPass[pNum])
      pNum++;
    if (pNum >= 3)
    {
      lcd.clear();
      pNum = curCol = curRow = 0;
      lcd.setCursor(curCol, curRow);
    }
    else
    {
      lcd.setCursor(curCol++, curRow);
      lcd.print(key);
    }
  }
  if (curCol >= 16 && curRow == 0)
  {
    curCol = 0;
    curRow = 1;
    lcd.setCursor(curCol, curRow);
  }
  if (curCol >= 16 && curRow == 1)
  {
    curCol = 0;
    curRow = 0;
    lcd.setCursor(curCol, curRow);
  }
  delay(50);
}
*/
