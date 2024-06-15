
#ifndef NEO_H
#define NEO_H

void colorWipe(Adafruit_NeoPixel &strip, uint32_t color, int wait);

void whiteOverRainbow(Adafruit_NeoPixel &strip, int whiteSpeed, int whiteLength);

void pulseWhite(Adafruit_NeoPixel &strip, uint8_t wait);

void rainbowFade2White(Adafruit_NeoPixel &strip, int wait, int rainbowLoops, int whiteLoops);

#endif
