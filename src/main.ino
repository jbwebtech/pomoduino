#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPR121.h>
#include <Adafruit_FRAM_I2C.h>

#define LED_PIN    6
#define LED_COUNT 24

// OLED display TWI address
void startupAnimation() {
  // Fade in from 0% to 100% brightness
  for(int brightness = 0; brightness <= 255; brightness += 5) {
    for(int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));
    }
    strip.show();
    delay(10);
  }
  // Fast chasing rainbow pattern
  for(long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256) {
    for(int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(20); // Pause for a moment
  }
  // Settle into a low white breathing glow
  for(int repeat = 0; repeat < 2; repeat++) {
    for(int brightness = 255; brightness >= 25; brightness -= 5) {
      for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));
      }
      strip.show();
      delay(10);
    }
    for(int brightness = 25; brightness <= 255; brightness += 5) {
      for(int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));
      }
      strip.show();
      delay(10);
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  // Initialize NeoPixel strip
  startupAnimation(); // Execute the startup animation
  Serial.begin(9600);
  
  // Initialize NeoPixel strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check your OLED address, it might be 0x3D for some models
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  // Initialize capacitive touch sensor
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Adafruit_FRAM_I2C fram = Adafruit_FRAM_I2C();
  if (!fram.begin()) {
    Serial.println("Could not find FRAM");
    while (1);
  }
  Serial.println("FRAM found!");
  loadSettings();
  displayMode(); // Display the default mode on startup
}
}

void idleModeAnimation() {
  const int minBrightness = 25; // Minimum brightness for breathing effect
  const int maxBrightness = 40; // Maximum brightness for breathing effect
  int breathingBrightness = minBrightness;
  bool increasing = true; // Direction of breathing effect
  int circlingLedIndex = 0; // Index of the circling LED
  unsigned long previousMillis = 0; // Store the last time the animation was updated
  const long interval = 100; // Interval at which to run the animation

  while(true) { // Replace with condition to exit idle mode
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // Breathing effect for all LEDs
      for(int i = 0; i < strip.numPixels(); i++) {
        if (i != circlingLedIndex) { // Apply breathing effect to all LEDs except the circling one
          strip.setPixelColor(i, strip.Color(breathingBrightness, breathingBrightness, breathingBrightness));
        }
      }

      // Update the breathing effect direction and brightness
      if (increasing) {
        breathingBrightness += 5;
        if (breathingBrightness >= maxBrightness) increasing = false;
      } else {
        breathingBrightness -= 5;
        if (breathingBrightness <= minBrightness) increasing = true;
      }

      // Circling LED effect
      strip.setPixelColor(circlingLedIndex, strip.Color(90, 90, 90)); // Set circling LED to a brighter white
      circlingLedIndex = (circlingLedIndex + 1) % strip.numPixels(); // Move to the next LED

      strip.show();
    }

    // Placeholder for checking exit condition from idle mode
  }
}

unsigned long touchStartTime = 0; // Time when the touch started
bool touchInProgress = false; // Whether a touch is currently in progress
const unsigned long longPressDuration = 500; // Duration in milliseconds that constitutes a long press

void loop() {
  checkInputs(); // Check for user inputs and handle them

  uint16_t touched = cap.touched();
  
  for (int i = 0; i < 3; i++) { // Check each button
    if (touched & (1 << i)) { // If button i is touched
      if (!touchInProgress) {
        touchInProgress = true;
        touchStartTime = millis();
      } else {
        if (millis() - touchStartTime > longPressDuration) {
          // Long press detected
          if (i == 1) { // Down button for brightness adjustment
            adjustBrightness();
            touchInProgress = false; // Reset touch detection
          } else if (i == 0) { // Up button for color adjustment
            adjustColor();
            touchInProgress = false; // Reset touch detection
          }
        }
      }
    } else {
      if (touchInProgress && (millis() - touchStartTime < longPressDuration)) {
        // Short press detected
        if (i == 1) { // Down button
          // Placeholder for Down button short press action
        } else if (i == 0) { // Up button
          // Placeholder for Up button short press action
        } else if (i == 2) { // Center button
          // Placeholder for Center button short press action
        }
      }
      touchInProgress = false;
    }
  }

  // Placeholder for additional loop logic
}
}

// Global variables for brightness and color settings
int currentBrightnessLevel = 2; // Default brightness level
int currentColorIndex = 0; // Default color index
const uint32_t colors[] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255), strip.Color(255, 255, 0), strip.Color(0, 255, 255), strip.Color(255, 0, 255), strip.Color(255, 255, 255), strip.Color(128, 0, 0), strip.Color(0, 128, 0), strip.Color(0, 0, 128), strip.Color(128, 128, 0), strip.Color(0, 128, 128), strip.Color(128, 0, 128), strip.Color(128, 128, 128), strip.Color(64, 64, 64), strip.Color(192, 192, 192)};
const int brightnessLevels[] = {50, 100, 150, 200, 255}; // Brightness levels

void adjustBrightness() {
  currentBrightnessLevel = (currentBrightnessLevel + 1) % 5; // Cycle through brightness levels
  updateLEDsBrightness();
}

void adjustColor() {
  currentColorIndex = (currentColorIndex + 1) % 16; // Cycle through colors
  updateLEDsColor();
}

void updateLEDsBrightness() {
  for(int i = 0; i < strip.numPixels(); i++) {
    uint32_t color = strip.getPixelColor(i);
    uint8_t r = (uint8_t)(color >> 16);
    uint8_t g = (uint8_t)(color >> 8);
    uint8_t b = (uint8_t)color;
    r = (r * brightnessLevels[currentBrightnessLevel]) / 255;
    g = (g * brightnessLevels[currentBrightnessLevel]) / 255;
    b = (b * brightnessLevels[currentBrightnessLevel]) / 255;
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void loadSettings() {
  currentBrightnessLevel = fram.read8(0);
  currentColorIndex = fram.read8(1);

  // Ensure loaded values are within expected ranges
  if (currentBrightnessLevel >= sizeof(brightnessLevels) / sizeof(brightnessLevels[0])) {
    currentBrightnessLevel = 2; // Default value
  }
  if (currentColorIndex >= sizeof(colors) / sizeof(colors[0])) {
    currentColorIndex = 0; // Default value
  }

  // Apply the loaded settings
  updateLEDsBrightness();
  updateLEDsColor();
}

