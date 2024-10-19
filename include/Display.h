#include <optional>

#ifndef Display_h
#define Display_h
#include "Arduino.h"

#ifndef MAXNUMDIGITS
#define MAXNUMDIGITS 4
#endif

typedef std::optional<u_int8_t> ledDigit;
const ledDigit digitOff = std::nullopt;
const ledDigit decimalPointOff = std::nullopt;

class Display
{

public:
    Display();
    void setup(const uint8_t latchPin, const uint8_t clockPin, const uint8_t dataPin, const uint8_t digitPins[], uint8_t numDigits);
    void set(const ledDigit nums[], ledDigit decimalOn);
    void refresh();
    void off();

protected:
    virtual void digitOn(uint8_t digit);
    virtual void digitOff(uint8_t digit);
    virtual void digitSet(uint8_t digit, uint8_t value);

    uint8_t latchPin, clockPin, dataPin;
    uint8_t digitOnValue, digitOffValue;

    uint8_t digitPins[MAXNUMDIGITS];
    uint8_t numDigits;
    ledDigit displayDigits[MAXNUMDIGITS]; // The active setting of each segment of each digit

private:
    ulong lastRefresh;
    uint8_t nextDigit;
    uint refreshEvery;
};

#endif // Display_h