#include "OLED.h"

OLED::OLED()
{
    w = 128;
    h = 64;
    textCursor.x = 0;
    textCursor.y = 0;
}
void OLED::setup(uint8_t w, uint8_t h)
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
    oled.clearDisplay();
    oled.setTextColor(WHITE);
}

void OLED::setHeader(String header)
{
    this->header = header;

    setTextCursor();
}

void OLED::setNumber(uint value, uint8_t decimals)
{
    this->value = value;
    this->decimals = decimals;

    Serial.printf("value: %u\n", value);
    Serial.printf("decimals: %u\n", decimals);

    setTextCursor();
}

void OLED::refresh()
{
    oled.clearDisplay();

    if (header != "")
    {
        oled.setCursor(0, 0);
        oled.setTextSize(headerTextSize);
        oled.println(header);
    }

    uint large = value / pow(10, decimals);
    uint small = value - large * pow(10, decimals);

    Serial.printf("large: %u\n", large);
    Serial.printf("small: %u\n", small);

    oled.setCursor(textCursor.x, textCursor.y);
    oled.setTextSize(largeTextSize);
    oled.print(large);
    oled.setTextSize(smallTextSize);
    oled.println(small);

    oled.display();
}

void OLED::setTextCursor()
{
    // 6x8
    uint16_t size = trunc(log10(value)) + 1;
    textCursor.x = (w - largeTextSize * 6 * (size - decimals) - smallTextSize * 6 * decimals) / 2;
    textCursor.y = header != "" ? (headerTextSize * 8 + headerMargin) : 0;
}
