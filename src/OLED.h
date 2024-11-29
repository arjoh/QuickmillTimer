#ifndef OLED_h
#define OLED_h

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class OLED
{
public:
    OLED();

    void setup(uint8_t w, uint8_t h);

private:
    Adafruit_SSD1306 oled;
};

#endif // OLED_h