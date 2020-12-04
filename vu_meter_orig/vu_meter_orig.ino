int VU;

void setup()
{
  //set baud rate
  Serial.begin(9600);
  //set pins attached to LEDs as outputs
  for (int i = 3; i  < 9; i++)
    pinMode(i, OUTPUT);
}
void loop()
{
  //set VU as value of pin A5 reading
  VU = analogRead(A5);
  
  //glow the LEDs depending on the ammount of sound detected by the electret  
  if (VU > 455 && VU < 555)
  {
    //glow first LED
    Clear(4);
    Glow(4);
  } 
  else if (VU > 378 && VU < 624)
  {
    //glow 2nd LED
    Clear(5);
    Glow(5);
  }
  else if (VU > 311 && VU < 693)
  {
    //glow 3rd LED
    Clear(6);
    Glow(6);
  }
  else if (VU > 244 && VU < 762)
  {
    //glow 4th LED
    Clear(7);
    Glow(7);
  }
  else if (VU > 177 && VU < 831)
  {
    //glow 5th LED
    Clear(8);
    Glow(8);
  }
  else if (VU < 177 || VU > 831)
  {
    //glow 6th LED
    Clear(9);
    Glow(9);
  }
  
}

//function to glow LEDs
void Glow(int initial)
{
  for (int i = 3; i < initial; i++)
    digitalWrite(i, HIGH);
}

//function to clear LEDs
void Clear(int initial)
{
  for (int i = initial; i < 9; i++)
    digitalWrite(i, LOW);
}

