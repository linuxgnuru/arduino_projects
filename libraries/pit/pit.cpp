#include "pit.h"

pit::pit(uint8_t pin)
{
	pinMode(pin, OUTPUT);
}

pit::printSer()
{
	Serial.println();
}

/*
pit::play(uint8_t pin)
{
	tone(pin, 
*/
