/*
  Button

Turns on and off a light emitting diode(LED) connected to digital  
pin 13, when pressing a pushbutton attached to pin 7. 


The circuit:
* LED attached from pin 13 to ground 
* pushbutton attached to pin 2 from +5V
* 10K resistor attached to pin 2 from ground

* Note: on most Arduinos there is already an LED on the board
attached to pin 13.


created 2005
by DojoDave <http://www.0j0.org>
modified 17 Jun 2009
by Tom Igoe

  http://www.arduino.cc/en/Tutorial/Button
*/

// constants won't change. They're used here to 
// set pin numbers:
//const int buttonPin = 2;     // the number of the pushbutton pin
int buttonPins[10];

int Buzzer1 = 2;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup()
{
  // initialize the piezo as output:
  pinMode(Buzzer1, OUTPUT);  
  // initialize the pushbutton pin as an input:
  for (int i = 0; i < 10; i++)
  {
    buttonPins[i] = i + 3;
    pinMode(buttonPins[i], INPUT);
  }
}

void loop()
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH)
  {
    // play th Music
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,450,225);
      delay(300);
      tone(Buzzer1,450,225);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,450,200);
      delay(300);
      tone(Buzzer1,600,300);
      delay(300);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,700,300);
      delay(300);
      tone(Buzzer1,700,300);
      delay(500);
      tone(Buzzer1,600,300);
      delay(300);
      tone(Buzzer1,400,200);
      delay(1000);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,650,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,650,200);
      delay(300);
      tone(Buzzer1,650,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(1000);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,600,300);
      delay(500);
      tone(Buzzer1,600,300);
      delay(500);
      tone(Buzzer1,800,300);
      delay(500);
      tone(Buzzer1,800,300);
      delay(500);
      tone(Buzzer1,400,200);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,400,200);
      delay(500);
      tone(Buzzer1,300,150);
      delay(500);
      tone(Buzzer1,300,150);
      delay(500);
      tone(Buzzer1,300,150);
      delay(500);
      tone(Buzzer1,300,150);
      delay(500);
      tone(Buzzer1,300,150);
      tone(Buzzer1,300,150);
      tone(Buzzer1,300,150);
      tone(Buzzer1,300,150);  

  } 
}
