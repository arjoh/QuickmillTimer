#include "OLED.h"

void OLED::setup(uint8_t w, uint8_t h)
{
    this->oled = Adafruit_SSD1306(w, h, &Wire, -1);

    if (!this->oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3D for 128x64
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
}
