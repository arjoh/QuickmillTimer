#include "Arduino.h"
#include "ezButton.h"
#include "internalLED.h"
#include "Display.h"

const uint8_t LED_PIN = /*4; */ LED_BUILTIN;
const uint8_t BUTTON_PIN = D6;

const int LED_ON = LED_PIN == LED_BUILTIN ? INTERNAL_LED_ON : HIGH;
const int LED_OFF = LED_PIN == LED_BUILTIN ? INTERNAL_LED_OFF : LOW;

bool running = false;

ezButton button(BUTTON_PIN);
u_char seconds = 5;
u_long started = 0;
u_long elapsed = 0;

// put function declarations here:
void updateDisplay(float, u_short);
float displaying = 0;
u_long displayUpdated = 0;
const u_short DISPLAY_EVERY = 100;
void display(float, u_char);
void refreshDisplay();
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

  const ledDigit numbers[] = {6, 6, 6};
  leds.set(numbers, decimalPointOff); // decimalPointOff);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);
  button.setDebounceTime(100); // set debounce time to 50 milliseconds

  display(seconds * 1000, 0);
}

void loop()
{
  button.loop();
  leds.refresh();
  updateDisplay(elapsed, DISPLAY_EVERY);

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

void updateDisplay(float value, u_short displayEvery)
{
  if (running)
  {
    u_long displayElapsed = millis() - displayUpdated;
    if (displayElapsed >= displayEvery)
    {
      display(elapsed, 1);
    }
  }
}

void display(float value, u_char decimals)
{

  if (value != displaying)
  {
    char str[4];
    dtostrf(value / 1000, decimals ? 4 : 2, decimals, str);
    Serial.println(String(str));
    displaying = value;
    displayUpdated = millis();
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
    display(seconds * 1000, 0);
  }
}
