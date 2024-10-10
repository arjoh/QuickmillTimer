#include <Arduino.h>
#include <ezButton.h>

int LED = 4; // LED_BUILTIN;
bool running = false;

ezButton button(5);
const int seconds = 5;
unsigned long started = 0;
unsigned long i = 0;

// put function declarations here:
void setRunning(bool);

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop()
{
  i++;
  button.loop(); // MUST call the loop() function first
  if (button.isReleased())
  {
    setRunning(!running);
  }
}

// put function definitions here:
void setRunning(bool to)
{
  running = to;
  digitalWrite(LED, running);
  Serial.printf("running: %s\n", running ? "true" : "false");
  started = running * millis();
  printf("started: %lu\n", started);
  printf("iterations: %lu\n", i);
  // if (running)
  // {
  //   started = millis();
  // }
  // else
  // {
  //   started = 0;
  // }
}