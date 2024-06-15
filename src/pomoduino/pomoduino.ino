// Pomoduino

#include <Adafruit_NeoPixel.h>

#include "neo.h"

#define LED_PIN    19
#define LED_COUNT  24
#define BRIGHTNESS 8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
}

void loop() {
  // Fill along the length of the strip in various colors...
  colorWipe(strip, strip.Color(255,   0,   0)     , 50); // Red
  colorWipe(strip, strip.Color(  0, 255,   0)     , 50); // Green
  colorWipe(strip, strip.Color(  0,   0, 255)     , 50); // Blue
  colorWipe(strip, strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)

  whiteOverRainbow(strip, 75, 5);

  pulseWhite(strip, 5);

  rainbowFade2White(strip, 2, 5, 3);
}
