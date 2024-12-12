#include "OTA.h"
#include <EEPROM.h>
#include "Arduino.h"
#include "ezButton.h"
#include "internalLED.h"
#include "OLED.h"
#include "config.h"

#ifndef NUM_DIGITS
#define NUM_DIGITS 3
#endif

#ifndef NUM_DECIMALS
#define NUM_DECIMALS 1
#endif

#ifndef HOSTNAME
#define HOSTNAME "quickmilltimer"
#endif

const int LED_ON = LED_PIN == LED_BUILTIN ? INTERNAL_LED_ON : HIGH;
const int LED_OFF = LED_PIN == LED_BUILTIN ? INTERNAL_LED_OFF : LOW;

ezButton button(BUTTON_PIN);

// running related settings/variables
uint8_t seconds = 25;
bool running = false;
ulong runningSince = 0;
ulong runningFor = 0;

// setting related settings/variables
ulong btnPressedAt = 0;
ulong btnPressedFor = 0;
uint resetAfter = 3000;
uint settingAfter = 4000;
float settingSpeed = 3;
bool setting = false;
bool connected = false;

ulong displaying = 0;

// Function definitions
void ready();
void display(ulong, int8_t);
void setRunning(bool);
void checkSetting();
void checkRunning();
void checkButton();
void checkWiFi();
void handleButtonPressed();
void handleButtonReleased();
void readSettings();
void writeSettings();

uint settingsAddress = 0;
uint settingsSize = 8;
struct
{
  uint8_t seconds;
} settings;

OLED oled = OLED();

void setup()
{
  Serial.begin(115200);
  Serial.println();

  oled.setup(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_DIGITS);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);
  button.setDebounceTime(100); // set debounce time to 50 milliseconds

  readSettings();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  ready();
}

void ready()
{
  // Just for fun..
  display(666, 0);
  oled.setHeader("QuickmillTimer");
  oled.setBlinking(true, 2664, 666, 333);
  while (oled.isBlinking)
  {
    oled.refresh();
  }
  // Let's get started!
  oled.setWiFiIcon(WiFiIcon::Disconnected);
  display(seconds * 10, NUM_DECIMALS);
  oled.refresh();
}

void loop()
{
  oled.refresh();
  checkRunning();
  checkButton();
  checkSetting();
  checkWiFi();
}

void checkSetting()
{
  // Don't respond to presses while running
  if (!running && btnPressedAt > 0)
  {
    btnPressedFor = millis() - btnPressedAt;
    if (!setting && btnPressedFor >= resetAfter)
    {
      Serial.printf("checkSetting: btnPressedFor=%lu\n", btnPressedFor);
      Serial.println("checkSetting: reset");
      seconds = 0;
      display(seconds, NUM_DECIMALS);
      oled.setBlinking(true, 1000, 200, 200);
      setting = true;
    }
    else if (setting && btnPressedFor >= settingAfter)
    {
      seconds = settingSpeed * (btnPressedFor - settingAfter) / 1000;
      display(seconds * 10, NUM_DECIMALS);
    }
  }
}
void checkRunning()
{
  if (running)
  {
    runningFor = millis() - runningSince;
    display(runningFor / 100, NUM_DECIMALS);
    if (running && seconds > 0 && runningFor >= seconds * 1000)
    {
      setRunning(false);
    }
  }
}

void checkButton()
{
  button.loop();
  if (button.isPressed())
  {
    handleButtonPressed();
  }
  if (button.isReleased())
  {
    handleButtonReleased();
  }
}

void checkWiFi()
{
  if (!connected && WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("checkWiFi: connected to %s\n", WIFI_SSID);
    Serial.printf("checkWiFi: localIP is %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("checkWiFi: connecting took %lums\n", millis());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    connected = true;
    oled.setWiFiIcon(WiFiIcon::Connected);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo."); });
    ElegantOTA.begin(&server);
    server.begin();
  }
  else if (connected && WiFi.status() != WL_CONNECTED)
  {
    oled.setWiFiIcon(WiFiIcon::Disconnected);
    Serial.printf("checkWiFi: connection to %s lost", WIFI_SSID);
    connected = false;
  }

  if (connected)
  {
    ElegantOTA.loop();
  }
}

void handleButtonPressed()
{
  btnPressedAt = millis();
  printf("btnPressedAt: %lu\n", btnPressedAt);
}

void handleButtonReleased()
{
  if (setting)
  {
    writeSettings();
    setting = false;
  }
  else
  {
    setRunning(!running);
  }
  btnPressedAt = 0;
  btnPressedFor = 0;
}

void readSettings()
{
  EEPROM.begin(settingsSize);
  EEPROM.get(settingsAddress, settings);
  seconds = settings.seconds;
  Serial.printf("readSettings: settings.seconds=%u\n", settings.seconds);
}

void writeSettings()
{
  settings.seconds = seconds;
  Serial.printf("writeSettings: settings.seconds=%u\n", seconds);
  EEPROM.put(settingsAddress, settings);
  EEPROM.commit();
}

void display(ulong value, int8_t decimals)
{
  if (value != displaying)
  {
    oled.setNumber(value, decimals);
    Serial.printf("display: %lu\n", value);
    displaying = value;
  }
}

void setRunning(bool to)
{
  // Don't do anything if running doesn't change.
  if (running == to)
  {
    return;
  }

  running = to;
  digitalWrite(LED_PIN, running ? LED_ON : LED_OFF);
  Serial.printf("setRunning: running=%s\n", running ? "true" : "false");
  if (!running)
  {
    Serial.printf("setRunning: elasped=%lu\n", runningFor);
  }
  runningSince = running ? millis() : 0;
  runningFor = 0;

  if (!running)
  {
    display(seconds * 10, NUM_DECIMALS);
  }
}
