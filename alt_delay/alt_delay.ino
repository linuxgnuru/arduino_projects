/*
Using a millis() timer

How to get similar delay. Timer is not a feature or function, it is simply
comparing millis() value regularily to determine when the desired time has elapsed.

Example 1

Implementing a simple timer

*/

unsigned long myTimer; // "ALWAYS use unsigned long for timers, not int"
                     // (variable declaration outside setup and loop, of course)
const byte PIN = 3;

void setup()
{
  pinMode(PIN, OUTPUT);
}

void loop()
{
  if (digitalRead(PIN) == LOW)
  {
    myTimer = millis();
    digitalWrite(PIN, HIGH); // "set timer"
  }
  if (millis() - myTimer >= 15000UL)
  {
    digitalWrite(PIN, LOW); //"timer expired"
  }
}

#if 0
/*
(the "UL" after the number is a syntax detail that is important when dealing with large
numbers in millis and micros, therefore it is shown although this is pseudo code)

Result: It works just as well as the delay version, but slightly differently: The timer
is constantly reset as long as the button is pushed. Thus only when releasing does it start
"count" the 15 seconds. If you push shortly inside the 15 seconds period, it starts counting
from anew. Notice in particular that digitalRead is looking at the input as fast as loop() runs.

The code is doing a digitalWrite OFF,even when the LED is not lit for every loop() pass, and
likewise digitalWrite HIGH as long as the button is pushed. Fortunatly this has no ill effects
(apart from taking a handfull of microseconds time)

Example 2

as attempt 2, with two buttons and LEDs: This is rather simple, we just "repeat" the above code.
*/

unsigned long TimerA; //    "ALWAYS use unsigned long for timers..."
unsigned long TimerB;

const int PIN = 3;
const int A = 5;
const int B = 6;

void setup()
{
  pinMode(PIN, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
}

void loop()
{
  if (digitalRead(A) == LOW)
  {
    TimerA = millis();
    digitalWrite(A, HIGH);
  }
  if (millis() - TimerA >= 15000UL)
  {
    digitalWrite(A, LOW);
  }
  if (digitalRead(B) == LOW)
  {
    TimerB = millis();
    digitalWrite(B, HIGH);
  }
  if (millis() - TimerB >= 15000UL)
  {
    digitalWrite(B, LOW);
  }
}

/*
Result: This works! Two separate "timers start and stop independently.

As shown in the single button/LED case, we are done with handling the A case in
just a few tens of microseconds, whether we are counting seconds or not, and thus
free to handle the B case. We thus examine and react to either buttons hundreds
of times each millisecond.

(Interesting sideissue: The 4 if statements can be freely rearranged - the end
effect is the same. (There are some subtle differences only active for the few
microseconds until loop() goes round again))

More complex millis() timers

The effect that the timer is to start on button push or button release, or if a button should be ignored once the timer has started may be a requirement. Timer code can be made to handle all variations by storing the desired state and skipping digitalRead. This will work fine with two buttons, too.

Example 3

Timer with State change
*/

boolean LEDon = false;
unsigned long Timer; //     "ALWAYS use unsigned long"

if (!LEDon && digitalRead(PIN) == LOW)
{
   Timer= millis(); // "set timer"
   digitalWrite(PIN, HIGH);
   LEDon = true;
}
if (LEDon && millis() - Timer >= 15000UL)
{
   digitalWrite(PIN, LOW);
   LEDon = false;
}

/*
Result: This will not do a digitalRead or reset the timer,
once the LED has turned on, ie we are starting the timer on
button push (like attempt 1 earlier). Likewise it only tests
the timer if the LED is on, and only turns it off once (which
make very little difference, so I would normally omit the LEDon
test in the 2nd part)

And this will work fine with two or more buttons and LEDs, as the
timers are independent and the code will not block.

Example 4

 One button, one LED - each with their own timer; A more usefull(?) example of doing two timing things "at once"
*/

boolean LEDon = false;
boolean buttondown = false;
unsigned long LEDtimer = 0; //        "ALWAYS use unsigned long..."
unsigned long buttontimer = 0; 

// "debounce 5 millisec"
if (millis() - buttontimer >= 5UL && !buttondown && digitalRead(Button) == LOW)
{
          buttondown = true;
         buttontimer = millis();
}
if (millis() - buttontimer >= 5UL && buttondown && digitalRead(Button) == HIGH)
{
         buttondown = false;
         buttontimer = millis();
}
if (!LEDon && buttondown)  // "can use: not buttondown"
{
  LEDtimer = millis();
  digitalWrite(PIN, HIGH);
  ledon = true;
}
if (millis() - LEDtimer >= 15000UL)
{
   digitalWrite(PIN, LOW);
   LEDon = false;
}
/*
Result: The button timer will purposfully skip digitalRead of the button for 5 millisec
after it has changed. This filters away the "noise" in any mechanical button. The Led on/off
code can be set to trigger on button down or up.

A challenge :)

If you use a previousbuttonup indicator instead of LEDon, you can ensure that the button has
to be released before a new timer cycle can start - but that is left as an exercise, and is
more about state transition than about timers.
*/
#endif
