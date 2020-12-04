#include <Servo.h>

class Sweeper
{
  Servo servo;              // the servo
  int pos;                  // current servo position 
  int increment;            // increment to move for each interval
  int  updateInterval;      // interval between updates
  unsigned long lastUpdate; // last update of position
  int myPin;                // my servo pin number
  boolean parked;

public: 
  Sweeper(int interval, int pnum)
  {
    parked = false;
    updateInterval = interval;
    increment = 2;
    myPin = pnum;
  }

  Sweeper(int interval)
  {
    parked = false;
    updateInterval = interval;
    increment = 2;
  }
  
  void Attach(int pin)
  {
    servo.attach(pin);
  }

  void Attach()
  {
    servo.attach(myPin);
  }
  
  void Detach()
  {
    servo.detach();
  }
  
  void Update()
  {
    if ((millis() - lastUpdate) > updateInterval)  // time to update
    {
      lastUpdate = millis();
      pos += increment;
      servo.write(pos);
      //Serial.println(pos);
      if ((pos >= 180) || (pos <= 1)) // end of sweep
      {
        // reverse direction
        increment = -increment;
      }
    }
  }

  int GetPos()
  {
    return pos;
  }

  void Park()
  {
    servo.write(2);
    parked = true;
  }

  void Park(int l_r)
  {
    servo.write((l_r ? 2 : 178));
    parked = true;
  }

  boolean isParked()
  {
    return parked;
  }
};
