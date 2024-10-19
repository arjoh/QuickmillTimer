#include "Display.h"

static const uint8_t digitValues[10]{
    B00000011, // 0
    B10011111, // 1
    B00100101, // 2
    B00001101, // 3
    B10011001, // 4
    B01001001, // 5
    B01000001, // 6
    B00011111, // 7
    B00000001, // 8
    B00001001  // 9
};

static const uint8_t segmentsOff = B11111111;
static const uint8_t decimalBit = 8;

Display::Display()
{
}

void Display::setup(const uint8_t latchPin, const uint8_t clockPin, const uint8_t dataPin, const uint8_t digitPins[], uint8_t numDigits)
{
    this->latchPin = latchPin;
    this->clockPin = clockPin;
    this->dataPin = dataPin;
    this->numDigits = numDigits;

    for (uint8_t digit = 0; digit < numDigits; digit++)
    {
        this->digitPins[digit] = digitPins[digit];
        pinMode(digitPins[digit], OUTPUT);
    }

    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);

    digitOffValue = LOW;
    digitOnValue = HIGH;

    off();

    lastRefresh = millis();
    nextDigit = 0;
    refreshEvery = 5;
}

void Display::set(const ledDigit numbers[], const ledDigit decimalOn = std::nullopt)
{
    for (int i = 0; i < numDigits; i++)
    {
        if (numbers[i].has_value())
        {
            displayDigits[i] = digitValues[numbers[i].value()] & (i == decimalOn.value_or(-1) ? 254 : 255);
        }
        else
        {
            displayDigits[i] = std::nullopt;
        }
    }
}

void Display::refresh()
{
    if (millis() - lastRefresh < refreshEvery)
    {
        return;
    }

    off();
    if (displayDigits[nextDigit].has_value())
    {
        digitSet(nextDigit, displayDigits[nextDigit].value());
    }
    else
    {
        digitOff(nextDigit);
    }
    nextDigit++;
    if (nextDigit == numDigits)
    {
        nextDigit = 0;
    }
    lastRefresh = millis();
}

void Display::off()
{
    for (uint8_t digit = 0; digit < numDigits; digit++)
    {
        digitOff(digit);
    }
}

void Display::digitOn(uint8_t digit)
{
    digitalWrite(digitPins[digit], digitOnValue);
}

void Display::digitOff(uint8_t digit)
{
    digitalWrite(digitPins[digit], digitOffValue);
}

void Display::digitSet(uint8_t digit, uint8_t value)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, value);
    digitalWrite(digitPins[digit], HIGH);
    digitalWrite(latchPin, HIGH);
}
