#ifndef BOARD_h
#define BOARD_h
#ifdef BOARD_NODEMCUV2
  #define INTERNAL_LED_ON  LOW
  #define INTERNAL_LED_OFF HIGH
#endif
#ifdef BOARD_ESP32_RELAY_AC_X2
#ifdef LED_BUILTIN
#undef LED_BUILTIN
#endif
  #define LED_BUILTIN 23
  #define INTERNAL_LED_ON HIGH
  #define INTERNAL_LED_OFF LOW
#endif
#endif