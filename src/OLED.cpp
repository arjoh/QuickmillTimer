#include "OLED.h"

OLED::OLED()
{
    w = 128;
    h = 64;
    textCursor.x = 0;
    textCursor.y = 0;
}
void OLED::setup(uint8_t w, uint8_t h, uint8_t numDigits)
{
    oled = Adafruit_SSD1306(w, h, &Wire, -1);
    if (!oled.begin(SSD1306_SWITCHCAPVCC,
                    0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    this->w = w;
    this->h = h;
    this->numDigits = numDigits;

    off();

    lastRefresh = millis();
    refreshEvery = 1;

    blinkFor = 0; // forever
    blinkOnFor = 500;
    blinkOffFor = 500;
    firstBlinked = 0;
    lastBlinked = 0;

    blinkOn = true;
    isBlinking = false;

    oled.clearDisplay();
    oled.setTextColor(WHITE, BLACK);
    oled.display();

    setTextCursor();
}

void OLED::setHeader(String header)
{
    this->header = header;
    this->changed = true;

    setTextCursor();
}

void OLED::setNumber(uint value, uint8_t decimals)
{
    this->value = value;
    this->decimals = decimals;
    this->changed = true;

    setTextCursor();
}

void OLED::setBlinking(bool blinking)
{
    setBlinking(blinkFor, 0);
}

void OLED::setBlinking(bool blinking, uint blinkFor)
{
    setBlinking(blinkFor, blinkFor, 500, 500);
}

void OLED::setBlinking(bool blinking, uint blinkFor, uint blinkOnFor,
                       uint blinkOffFor)
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
}

void OLED::blink()
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
void OLED::refresh()
{
    if (isBlinking)
    {
        blink();
        if (!blinkOn)
        {
            clearText();
            return;
        }
    }
    else if (!changed)
    {
        return;
    }

    oled.clearDisplay();

    if (header != "")
    {
        oled.setCursor(0, 0);
        oled.setTextSize(headerTextSize);
        oled.println(header);
    }

    uint8_t size = value < 10 ? 1 + decimals : trunc(log10(value)) + 1;
    uint large = value / pow(10, decimals);
    uint small = value - large * pow(10, decimals);

    oled.setCursor(textCursor.x, textCursor.y);
    oled.setTextSize(largeTextSize);
    for (uint8_t i = 0; i < numDigits - size; i++)
    {
        oled.print(" ");
    }
    oled.print(large);

    if (decimals > 0)
    {
        oled.setTextSize(smallTextSize);
        oled.print(small);
    }

    // oled.println();
    oled.display();
    changed = false;
}

void OLED::clearText()
{
    oled.setCursor(textCursor.x, textCursor.y);
    oled.setTextSize(largeTextSize);
    for (uint8_t i = 0; i < numDigits; i++)
    {
        oled.print(" ");
    }
    oled.display();
}

void OLED::off()
{
    oled.clearDisplay();
    oled.display();
}

void OLED::setTextCursor()
{
    // 6x8
    // uint16_t size = trunc(log10(value)) + 1;
    textCursor.x = (w - largeTextSize * 6 * numDigits) / 2;
    textCursor.y = header != "" ? (headerTextSize * 8 + headerMargin) : 0;
}
