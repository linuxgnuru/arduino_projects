#include <SPI.h>

class Flasher
{
	// Class Member Variables
	// These are initialized at startup
    int myLatch;
	int ledPin;      // the number of the LED pin
    int chipN;
	unsigned long OnTime;     // milliseconds of on-time
	unsigned long OffTime;    // milliseconds of off-time

	// These maintain the current state
	int ledState;             		// ledState used to set the LED
	unsigned long previousMillis;  	// will store last time LED was updated

	byte data[2];

  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int latch, int pin, long on, long off)
  {
    if (pin > 7)
    {
      chipN = 1;
      ledPin = abs(pin - 15);
    }
    else
    {
      chipN = 0;
	  ledPin = pin;
    }
    myLatch = latch;
    data[0] = data[1] = 0b00000000;
	pinMode(myLatch, OUTPUT);     
	//pinMode(ledPin, OUTPUT);     
	  
	OnTime = on;
	OffTime = off;
	
	ledState = LOW; 
	previousMillis = 0;
  }

  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
    	ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      
      //digitalWrite(ledPin, ledState);  // Update the actual LED
      bitWrite(data[chipN], ledPin, ledState);
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      //digitalWrite(ledPin, ledState);	  // Update the actual LED
      bitWrite(data[chipN], ledPin, ledState);
      digitalWrite(myLatch, LOW);
      SPI.transfer(data[0]);
      SPI.transfer(data[1]);
      digitalWrite(myLatch, HIGH);
    }
  }
};

/*
Flasher led1(12, 100, 400);
Flasher led2(13, 350, 350);

void setup()
{
}

void loop()
{
	led1.Update();
	led2.Update();
}
*/
