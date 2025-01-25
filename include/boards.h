#ifndef BOARD_h
#define BOARD_h
#ifdef ESP8266
  #define INTERNAL_LED_ON  LOW
  #define INTERNAL_LED_OFF HIGH
#endif
#ifdef ESP32
#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
  #define LED_BUILTIN 23
  #define INTERNAL_LED_ON HIGH
  #define INTERNAL_LED_OFF LOW
#endif
#endif