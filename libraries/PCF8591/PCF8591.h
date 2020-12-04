//
//    FILE: PCF8591.H
//  AUTHOR: Rob Tillaart
//    DATE: 02-febr-2013
// VERSION: 0.1.08
// PURPOSE: I2C PCF8591 library for Arduino
//     URL: http://forum.arduino.cc/index.php?topic=184800
//
// HISTORY:
// see PCF8591.cpp file
//

#ifndef _PCF8591_H
#define _PCF8591_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define PCF8591_LIB_VERSION "0.1.08"

#define PCF8591 (0x90 >> 1) // I2C bus address

#define PCF8591_OK          0x00
#define PCF8591_PIN_ERROR   0x81
#define PCF8591_I2C_ERROR   0x82


class PCF8591
{
public:
    explicit PCF8591(const uint8_t deviceAddress);

    void begin(uint8_t val=0xFF);

    uint8_t read8();
    uint8_t read(uint8_t pin);
    uint8_t value() const { return _dataIn; };

    void write8(const uint8_t value);
    void write(const uint8_t pin, const uint8_t value);
    uint8_t valueOut() const { return _dataOut; }

    int lastError();

private:
    uint8_t _address;
    uint8_t _dataIn;
    uint8_t _dataOut;
    int _error;
};

#endif
//
// END OF FILE
//
