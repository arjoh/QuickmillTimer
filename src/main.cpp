#include <EEPROM.h>
#include <cmath>

#include "Arduino.h"
#include "ezButton.h"
#include "internalLED.h"
#include "OLED.h"

const uint8_t LED_PIN = /*4; */ LED_BUILTIN;
const uint8_t BUTTON_PIN = D6;

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

ulong displaying = 0;

// Function definitions
void ready();
void display(ulong, uint8_t);
void setRunning(bool);
void checkSetting();
void checkRunning();
void checkButton();
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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define NUM_DIGITS 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
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

  ready();
}

void ready()
{
  // Just for fun..
  display(666, 0);
  oled.setHeader("QuickmillTimer");
  oled.refresh();
  oled.setBlinking(true, 2664, 666, 333);
  while (oled.isBlinking)
  {
    oled.refresh();
  }
  // Let's get started!
  display(seconds * 10, 1);
}

void loop()
{
  oled.refresh();
  checkRunning();
  checkButton();
  checkSetting();
}

void checkSetting()
{
  // Don't respond to presses while running
  if (!running && btnPressedAt > 0)
  {
    btnPressedFor = millis() - btnPressedAt;
    if (!setting && btnPressedFor >= resetAfter)
    {
      Serial.printf("btnPressedFor: %lu\n", btnPressedFor);
      Serial.println("reset");
      seconds = 0;
      display(seconds, 1);
      oled.setBlinking(true, 1000, 200, 200);
      setting = true;
    }
    else if (setting && btnPressedFor >= settingAfter)
    {
      seconds = settingSpeed * (btnPressedFor - settingAfter) / 1000;
      display(seconds * 10, 1);
    }
  }
}
void checkRunning()
{
  if (running)
  {
    runningFor = millis() - runningSince;
    display(runningFor / 100, 1);
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
}

void writeSettings()
{
  settings.seconds = seconds;
  EEPROM.put(settingsAddress, settings);
  EEPROM.commit();
}

void display(ulong value, uint8_t decimals)
{
  if (value != displaying)
  {
    oled.setNumber(value, decimals);
    Serial.println(value);
    displaying = value;
    // oled.refresh(); // TODO move to loop and leave refresh interval to OLED.h
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
  Serial.printf("running: %s\n", running ? "true" : "false");
  if (!running)
  {
    Serial.printf("elasped: %lu\n", runningFor);
  }
  runningSince = running ? millis() : 0;
  runningFor = 0;

  if (!running)
  {
    display(seconds * 10, 1);
  }
}
