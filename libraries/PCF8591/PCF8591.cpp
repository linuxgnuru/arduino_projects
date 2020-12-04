#include "PCF8591.h"

#include <Wire.h>

PCF8591::PCF8591(const uint8_t deviceAddress)
{
    _address = deviceAddress;
    _dataIn = 0;
    _dataOut = 0xFF;
    _error = PCF8591_OK;
}

void PCF8591::begin(uint8_t val)
{
  Wire.begin();
  //PCF8591::write8(val);
}

// removed Wire.beginTransmission(addr);
// with  @100KHz -> 265 micros()
// without @100KHz -> 132 micros()
// without @400KHz -> 52 micros()
// TODO @800KHz -> ??
uint8_t PCF8591::read8()
{
    if (Wire.requestFrom(_address, (uint8_t)1) != 1)
    {
        _error = PCF8591_I2C_ERROR;
        return _dataIn; // last value
    }
#if (ARDUINO <  100)
    _dataIn = Wire.receive();
#else
    _dataIn = Wire.read();
#endif
    return _dataIn;
}

uint8_t PCF8591::read(const uint8_t pin)
{
    if (pin > 7)
    {
        _error = PCF8591_PIN_ERROR;
        return 0;
    }
    PCF8591::read8();
    return (_dataIn & (1 << pin)) > 0;
}

void PCF8591::write(const uint8_t pin, const uint8_t value)
{
    if (pin > 7)
    {
        _error = PCF8591_PIN_ERROR;
        return;
    }
    if (value == LOW)
    {
        _dataOut &= ~(1 << pin);
    }
    else
    {
        _dataOut |= (1 << pin);
    }
    //write8(_dataOut);
}

int PCF8591::lastError()
{
    int e = _error;
    _error = PCF8591_OK;
    return e;
}
