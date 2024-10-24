#include "Arduino.h"
#include "ezButton.h"
#include "internalLED.h"
#include "Display.h"
#include "Timer.h"

const uint8_t LED_PIN = /*4; */ LED_BUILTIN;
const uint8_t BUTTON_PIN = D6;

const int LED_ON = LED_PIN == LED_BUILTIN ? INTERNAL_LED_ON : HIGH;
const int LED_OFF = LED_PIN == LED_BUILTIN ? INTERNAL_LED_OFF : LOW;

bool running = false;

ezButton button(BUTTON_PIN);
uint8_t seconds = 25;
ulong started = 0;
ulong elapsed = 0;
ulong displaying = 0;

// Function definitions
void ready();
void display(ulong, uint8_t);
void setRunning(bool);

#define LATCH_PIN 15
#define CLOCK_PIN 14
#define DATA_PIN 13

#define DIGIT1_PIN D1
#define DIGIT2_PIN D2
#define DIGIT3_PIN D3

int display_timer;

Display leds = Display();

void setup()
{
  Serial.begin(115200);
  Serial.println();

  uint8_t digitPins[] = {DIGIT1_PIN, DIGIT2_PIN, DIGIT3_PIN};
  leds.setup(LATCH_PIN, CLOCK_PIN, DATA_PIN, digitPins, 3);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);
  button.setDebounceTime(100); // set debounce time to 50 milliseconds

  ready();
}

void ready()
{
  // Just for fun..
  display(666, 0);
  leds.setBlinking(true, 2600, 666, 334);
  while (leds.isBlinking)
  {
    leds.refresh();
  }
  // Let's get started!
  display(seconds * 10, 1);
}

void loop()
{
  button.loop();
  leds.refresh();

  if (running)
  {
    display(elapsed / 100, 1);
  }

  elapsed = millis() - started;

  if (running && elapsed >= seconds * 1000)
  {
    setRunning(false);
    return;
  }

  if (button.isReleased())
  {
    setRunning(!running);
  }
}

void display(ulong value, uint8_t decimals)
{
  if (value != displaying)
  {
    leds.setNumber(value, decimals);
    displaying = value;
  }
}

// put function definitions here:
void setRunning(bool to)
{
  running = to;
  digitalWrite(LED_PIN, running ? LED_ON : LED_OFF);
  Serial.printf("running: %s\n", running ? "true" : "false");
  if (!running)
  {
    Serial.printf("elasped: %lu\n", elapsed);
  }
  started = running * millis();
  elapsed = 0;

  if (!running)
  {
    display(seconds * 10, 1);
  }
}
