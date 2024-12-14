#ifndef OTA_h
#define OTA_h

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <ESPAsyncWebServer.h>

#include <ElegantOTA.h>

AsyncWebServer server(80);

#endif // OTA_h