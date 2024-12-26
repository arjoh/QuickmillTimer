#ifndef OLED_h
#define OLED_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

typedef struct
{
    int16_t x;
    int16_t y;
} Cursor;

enum class WiFiIcon
{
    Connected,
    Disconnected,
    NA
};

class OLED
{
public:
    OLED();

    void setup(uint8_t w, uint8_t h, uint8_t numDigits);
    void setHeader(String header);
    void setNumber(uint number, int8_t decimals);

    void setWiFiIcon(WiFiIcon icon);
    void setBlinking(bool blinking);
    void setBlinking(bool blinking, uint blinkFor);
    void setBlinking(bool blinking, uint blinkFor, uint blinkOnFor, uint blinkOffFor);
    bool isBlinking;

    void refresh();
    void clearText();
    void off();

    Adafruit_SSD1306 oled;

protected:
    virtual void blink();
    uint8_t numDigits;

    uint8_t h;
    uint8_t w;

    uint8_t largeTextSize = 6;
    uint8_t smallTextSize = 4;

    String header;

    uint8_t headerTextSize = 1;
    uint8_t headerMargin = 9;

    uint value;
    int8_t decimals;

    bool blinkOn;

private:
    void setTextCursor();
    Cursor textCursor = Cursor();
    ulong lastRefresh;
    uint refreshEvery;
    bool changed = false;

    uint blinkOnFor;
    uint blinkOffFor;

    uint blinkFor;
    ulong firstBlinked;
    ulong lastBlinked;

    WiFiIcon wiFiIcon;
};

#endif // OLED_h