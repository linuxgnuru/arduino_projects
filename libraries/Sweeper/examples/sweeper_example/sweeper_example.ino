#include <Sweeper.h>

Sweeper sweeper1(15);
Sweeper sweeper2(25);

void setup()
{
   Serial.begin(9600);
   sweeper1.Attach(9);
   sweeper2.Attach(10);
}

void loop()
{
    sweeper1.Update();
    sweeper2.Update();
}
