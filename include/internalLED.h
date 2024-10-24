#if defined(__arm__) || defined(ARDUINO_ARCH_AVR)
  #define INTERNAL_LED_ON  HIGH
  #define INTERNAL_LED_OFF LOW
#else
  #define INTERNAL_LED_ON LOW
  #define INTERNAL_LED_OFF HIGH
#endif