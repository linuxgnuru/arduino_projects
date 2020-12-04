#include <SPI.h>

const int LATCH = 10;
const int rowPin[] = {
  7, 6, 5, 4
};
byte data1 = 0b00000000, data2 = 0b00000000;

void setup()
{
  SPI.begin();
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, HIGH);
  for (int i = 0; i < 4; i++)
  {
    pinMode(rowPin[i], OUTPUT);
    digitalWrite(rowPin[i], HIGH);
  }
  Serial.begin(9600);
  Serial.println(F("Enter chip num (0 / 1) and led (0-7)"));

  digitalWrite(LATCH, LOW);
  SPI.transfer(data1);
  SPI.transfer(data2);
  digitalWrite(LATCH, HIGH);
}


void loop()
{
  int row, col;
  //#if 0
  int chip = 0;
  int myCol = 0;
  digitalWrite(rowPin[1], LOW);
  while (Serial.available() > 0)
  {
    //int val = Serial.read();
    //myRow = val - 48;
    chip = Serial.parseInt();
    myCol = Serial.parseInt();
    if (Serial.read() == '\n')
    {
      chip = constrain(chip, 0, 1);
      myCol = constrain(myCol, 0, 7);
      Serial.print("chip: ");
      Serial.print(chip);
      Serial.print(" col: ");
      Serial.println(myCol);
      Serial.print("data1 data2: ");
      Serial.print(data1, BIN);
      Serial.print(" ");
      Serial.println(data2, BIN);
      //bitSet((chip == 0 ? data1 : data2), myCol);
      bitWrite((chip == 0 ? data1 : data2), myCol, !bitRead((chip == 0 ? data1 : data2), myCol));
      digitalWrite(rowPin[0], LOW);
      digitalWrite(LATCH, LOW);
      SPI.transfer(data1);
      SPI.transfer(data2);
      digitalWrite(LATCH, HIGH);
      digitalWrite(rowPin[0], HIGH);
      Serial.println(F("Enter chip num (0 / 1) and led (0-7)"));
    }
  }
#if 0
  for (row = 0; row < 4; row++)
  {
    digitalWrite(rowPin[row], LOW);
    if (row == 0)
      digitalWrite(rowPin[3], HIGH);
    else
      digitalWrite(rowPin[row - 1], HIGH);
    for (col = 0; col < 16; col++)
    {
      if (col < 8)
        bitSet(data2, col);
      else
        bitSet(data1, abs(col - 8));
      digitalWrite(LATCH, LOW);
      SPI.transfer(data1);
      SPI.transfer(data2);
      digitalWrite(LATCH, HIGH);
      delay(25);
    }
    for (col = 16; col > -1; col--)
    {
      if (col < 8)
        bitClear(data2, col);
      else
        bitClear(data1, abs(col - 8));
      digitalWrite(LATCH, LOW);
      SPI.transfer(data1);
      SPI.transfer(data2);
      digitalWrite(LATCH, HIGH);
      delay(25);
    }
  }
#endif
  delay(200);
  digitalWrite(rowPin[1], HIGH);
}



