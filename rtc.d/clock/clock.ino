/*  Simple internal clock demo: by Joseph Unik aka Relic1974
    Uses analog PWM output of 490Hz with a 50% duty cycle to
    keep very accurate time ;).  Connect an LED to pin 13 to
    watch seconds blink.  Connect a jumper from Analog pin 0
    to Digital Pin 2 (interrupt 0). Minutes output to serial
    monitor.  http://www.planetxresearch.com 'Arduino' logo
    for more projects and tricks...
   
   
    (Feel free to use this code to expand into a fully-functional
    clock or other project under Creative Commons ;)
*/

int clockInt = 0;            // digital pin 2 is now interrupt 0
int masterClock = 0;         // counts rising edge clock signals
int seconds = 0;             // variable
int minutes = 0;             // variable
int ledPin = LED_BUILTIN;

void setup()
{
  attachInterrupt(0, clockCounter, CHANGE);
      //  clockInt is our interrupt, clockCounter function is called when
      //  invoked on a RISING clock edge
  analogReference(DEFAULT);
  analogWrite(3, 127);
  pinMode(ledPin, OUTPUT);
  Serial.begin(57600);
  analogWrite(0, 127);   // this starts our PWM 'clock' with a 50% duty cycle
  pinMode(8, OUTPUT);
}

void clockCounter()      // called by interrupt
{
  int ledpinstate = 0;
  masterClock ++;        // with each clock rise add 1 to masterclock count
  if(masterClock == 979) // 490Hz reached     
  {                         
    seconds ++;          // after one 490Hz cycle add 1 second ;)
    masterClock = 0;     // Reset after 1 second is reached
    ledpinstate = !ledpinstate;
    //tone(13, 100, 500);  // using tone to pulse LED without delay call ;)
   }
   digitalWrite(ledPin, ledpinstate);
  return;
}

void loop()
{
  if(seconds == 60)      // NOW GETTING IN TO REAL TIME KEEPING
  {
    minutes ++;          // increment minutes by 1
    seconds = 0;         // reset the seconds variable
    Serial.print("Minutes = ");
    Serial.println(minutes);
    if (minutes % 10 == 0)
    {
       digitalWrite(8, HIGH);
    }
  }
}
