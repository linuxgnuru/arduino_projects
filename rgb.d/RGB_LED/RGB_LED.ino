/*************************************************************************/
const int redPin = 11;    // R petal on RGB LED module connected to digital pin 11
const int greenPin = 10;  // G petal on RGB LED module connected to digital pin 9
const int bluePin = 9;    // B petal on RGB LED module connected to digital pin 10
/**************************************************************************/

void setup()
{
         pinMode(redPin, OUTPUT); // sets the redPin to be an output
         pinMode(greenPin, OUTPUT); // sets the greenPin to be an output
         pinMode(bluePin, OUTPUT); // sets the bluePin to be an output
}

/***************************************************************************/
void loop()  // run over and over again
{
  unsigned int rgb[3];

  // Start off with red.
  rgb[0] = 255;
  rgb[1] = 0;
  rgb[2] = 0;
  // Choose the colours to increment and decrement.
  for (int decrement = 0; decrement < 3; decrement += 1)
  {
    int increment = decrement == 2 ? 0 : decrement + 1;
    // cross-fade the two colors.
    for (int i = 0; i < 255; i += 1)
    {
      rgb[decrement] -= 1;
      rgb[increment] += 1;
      color(rgb[0], rgb[1], rgb[2]);
      delay(5);
    }
  }
  /*
  int r, g, b;
  for (r = 0; r < 255; r += 25)
  {
    for (g = 0; g < 255; g += 25)
    {
      for (b = 0; b < 255; b += 25)
      {
        color(r, g, b);
        delay(10);
      }
    }
  }
  */
#ifdef DEMO
          // Basic colors:  
          color(255, 0, 0); // turn the RGB LED red 
          delay(1000); // delay for 1 second  
          color(0, 255, 0); // turn the RGB LED green  
          delay(1000); // delay for 1 second  
          color(0, 0, 255); // turn the RGB LED blue  
          delay(1000); // delay for 1 second 
          // Example blended colors:  
          delay(1000); // delay for 1 second  
          color(237, 109, 0); // turn the RGB LED orange  
          delay(1000); // delay for 1 second  
          color(255, 215, 0); // turn the RGB LED yellow  
          delay(1000); // delay for 1 second  
          color(34, 139, 34); // turn the RGB LED green  
          delay(1000); // delay for 1 second 
          delay(1000); // delay for 1 second
          color(0,46,90); // turn the RGB LED  indigo 
          delay(1000); // delay for 1 second
          color(128,0,128); // turn the RGB LED purple  
          delay(1000); // delay for 1 second
#endif
}

/******************************************************/
void color(unsigned char red, unsigned char green, unsigned char blue)     // the color generating function
{
          analogWrite(redPin, red);
          analogWrite(bluePin, blue);
          analogWrite(greenPin, green);
}
/******************************************************/

