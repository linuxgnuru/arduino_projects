String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
  for (int i = 2; i < 14; i++)
    pinMode(i, OUTPUT);
  for (int i = A0; i < A6; i++)
    pinMode(i, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }  inputString.reserve(200);
  Serial.println("Enter pin number (2-18 only; A0 = 14 A5 = 18)");
  //Serial.print("A0: ");  Serial.print(A0);  Serial.print(" A5: ");  Serial.println(A5);
}

void loop()
{
  int num;
  while (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar))
      inputString += (char)inChar;
    if (inChar == '\n')
    {
      Serial.println(inputString);
      num = inputString.toInt();
      digitalWrite(num, (digitalRead(num) ? LOW : HIGH));
      inputString = "";
    }
  }
}

#if 0
for (int i = 2; i < 14; i++)
{
  digitalWrite(i, HIGH);
  delay(500);
}
for (int i = A0; i < A6; i++)
{
  digitalWrite(i, HIGH);
  delay(500);
}
#endif

