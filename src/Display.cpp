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
    refreshEvery = 1;

    blinkFor = 0; // forever
    blinkOnFor = 500;
    blinkOffFor = 500;
    firstBlinked = 0;
    lastBlinked = 0;

    blinkOn = true;
    isBlinking = false;
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

void Display::setNumber(uint value, uint8_t decimals)
{
    Serial.println(value);
    ledDigit numbers[3];

    for (uint8_t i = 0; i < numDigits; i++)
    {
        if (numDigits - i - 1 == 0 && value % 10 == 0)
        {
            numbers[numDigits - i - 1] = ledsOff;
        }
        else
        {
            numbers[numDigits - i - 1] = value % 10;
        }
        value /= 10;
    }
    set(numbers, decimals > 0 ? numDigits - 1 - decimals : ledsOff);
}

void Display::setBlinking(bool blinking)
{
    setBlinking(blinkFor, 0);
}

void Display::setBlinking(bool blinking, uint blinkFor)
{
    setBlinking(blinkFor, blinkFor, 500, 500);
}

void Display::setBlinking(bool blinking, uint blinkFor, uint blinkOnFor, uint blinkOffFor)
{
    this->isBlinking = blinking;
    this->blinkFor = blinkFor;
    this->blinkOnFor = blinkOnFor;
    this->blinkOffFor = blinkOffFor;

    if (blinking)
    {
        this->firstBlinked = millis();
        this->lastBlinked = this->firstBlinked;
    }
    else
    {
        this->firstBlinked = 0;
        this->lastBlinked = 0;
    }

    Serial.printf("setBlinking(%s);\n", blinking ? "true" : "false");
    Serial.printf("\tthis->isBlinking = %d;\n", this->isBlinking);
    Serial.printf("\tthis->blinkFor = %d;\n", this->blinkFor);
    Serial.printf("\tthis->blinkOnFor = %d;\n", this->blinkOnFor);
    Serial.printf("\tthis->blinkOffFor = %d;\n", this->blinkOffFor);
    Serial.printf("\tthis->firstBlinked = %lu;\n", this->firstBlinked);
    Serial.printf("\tthis->lastBlinked = %lu;\n", this->lastBlinked);
}

void Display::blink()
{
    if (isBlinking)
    {
        if (blinkFor > 0 && millis() - firstBlinked >= blinkFor)
        {
            setBlinking(false);
        }

        if (blinkOn && millis() - lastBlinked >= blinkOnFor)
        {
            blinkOn = false;
            lastBlinked = millis();
        }

        if (!blinkOn && millis() - lastBlinked >= blinkOffFor)
        {
            blinkOn = true;
            lastBlinked = millis();
        }
    }
}

void Display::refresh()
{
    if (isBlinking)
    {
        blink();
        if (!blinkOn)
        {
            off();
            return;
        }
    }

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
    digitOn(digit);
    digitalWrite(latchPin, HIGH);
}
