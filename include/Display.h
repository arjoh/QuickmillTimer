#include <optional>

#ifndef Display_h
#define Display_h
#include "Arduino.h"

#ifndef MAXNUMDIGITS
#define MAXNUMDIGITS 4
#endif

typedef std::optional<uint8_t> ledDigit;
const ledDigit ledsOff = std::nullopt;

class Display
{

public:
    Display();

    void setup(const uint8_t latchPin, const uint8_t clockPin, const uint8_t dataPin, const uint8_t digitPins[], uint8_t numDigits);

    void set(const ledDigit numbers[], ledDigit decimalOn);
    void setNumber(uint value, uint8_t decimals);

    void setBlinking(bool blinking);
    void setBlinking(bool blinking, uint blinkFor);
    void setBlinking(bool blinking, uint blinkFor, uint blinkOnFor, uint blinkOffFor);
    bool isBlinking;

    void refresh();
    void off();

protected:
    virtual void blink();
    virtual void digitOn(uint8_t digit);
    virtual void digitOff(uint8_t digit);
    virtual void digitSet(uint8_t digit, uint8_t value);

    uint8_t latchPin, clockPin, dataPin;
    uint8_t digitOnValue, digitOffValue;

    uint8_t digitPins[MAXNUMDIGITS];
    uint8_t numDigits;
    ledDigit displayDigits[MAXNUMDIGITS]; // The active setting of each segment of each digit

    bool blinkOn;

private:
    ulong lastRefresh;
    uint8_t nextDigit;
    uint refreshEvery;

    uint blinkOnFor;
    uint blinkOffFor;

    uint blinkFor;
    ulong firstBlinked;
    ulong lastBlinked;
};

#endif // Display_h