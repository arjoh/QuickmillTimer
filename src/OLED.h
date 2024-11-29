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

class OLED
{
public:
    OLED();

    void setup(uint8_t w, uint8_t h);
    void setHeader(String header);
    void setNumber(uint number, uint8_t decimals);

    void refresh();

protected:
private:
    Adafruit_SSD1306 oled;

    uint8_t h;
    uint8_t w;

    uint8_t largeTextSize = 6;
    uint8_t smallTextSize = 5;

    String header;
    
    uint8_t headerTextSize = 1;
    uint8_t headerMargin = 4;

    uint value;
    uint8_t decimals;
    void setTextCursor();
    Cursor textCursor = Cursor();
};

#endif // OLED_h