#include "FastLED.h"
#define NUM_LEDS 50
CRGB leds[NUM_LEDS];
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;
uint8_t sunrise = 6; 
uint8_t sunset = 17; 
void setup() {
  FastLED.addLeds<APA102, 3, 13, BGR>(leds, NUM_LEDS); // BGR ordering is typical
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();
  int currentHour = now.hour();
  if (currentHour < sunset && currentHour > sunrise) {
    animation(blend(CRGB(100, 90, 5), CRGB(100, 50, 255), 0), 10, 10, 50);
  } else if (currentHour > sunset || currentHour < sunrise) {
    animation(blend(CRGB(100, 90, 5), CRGB(100, 50, 255), 255), 10, 10, 50);
  } else if (currentHour == sunrise) {
    int currentMin = now.minute();
    animation(blend(CRGB(100, 90, 5), CRGB(100, 50, 255), map(currentMin, 0, 60, 0, 255)), 10, 10, 50);
  } else if (currentHour == sunset) {
    int currentMin = now.minute();
    animation(blend(CRGB(100, 90, 5), CRGB(100, 50, 255), map(currentMin, 0, 60, 255, 0)), 10, 10, 50);
  }
}

void animation(CRGB color, int EyeSize, int SpeedDelay, int damp) {
  for (int i = ((NUM_LEDS - EyeSize) / 2); i >= 0; i--) {
    setAll(constrain(color.r - damp, 0, 255), constrain(color.g - damp, 0, 255), constrain(color.b - damp, 0, 255));
    for (int j = 1; j <= EyeSize; j++) {
      int red = constrain(color.r + random(255), 0, 255);
      setPixel(i + j, red, color.g, color.b);
    }
    setPixel(NUM_LEDS - i, color.r / 10, color.g / 10, color.b / 10);
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, color.r, color.g, color.b);
    }
    showStrip();
    delay(SpeedDelay);
  }
  for (int i = 0; i <= ((NUM_LEDS - EyeSize) / 2); i++) {
    setAll(constrain(color.r - damp, 0, 255), constrain(color.g - damp, 0, 255), constrain(color.b - damp, 0, 255));
    for (int j = 1; j <= EyeSize; j++) {
      int red = constrain(color.r + random(255), 0, 255);
      setPixel(i + j, red, color.g, color.b);
    }
    for (int j = 1; j <= EyeSize; j++) {
      setPixel(NUM_LEDS - i - j, color.r, color.g, color.b);
    }
    showStrip();
    delay(SpeedDelay);
  }
}
void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
