#ifndef config_h
#define config_h
#include "secrets.h"

// General settings
#if defined(ESP8266)
#define LED_PIN LED_BUILTIN
#define BUTTON_PIN D6
#elif defined(ESP32)
#define LED_PIN 23
#define RELAY_PIN 16
#endif

// #define RELAY_PIN LED_BUILTIN
// #define HOSTNAME "QuickmillTimer"
// #define NUM_DIGITS 3
// #define NUM_DECIMALS 1

// OLED display
#define OLED_DISPLAY
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#endif