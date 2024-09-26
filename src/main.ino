/*
Created by: Ben Zhou
*/

#define LED 18
#define NUM_LEDS 259
#include <FastLED.h>

CRGB LEDs[NUM_LEDS];

int step = 0;

bool fadeLED() {
    
    if (step > 255) {
        step = 0;
        return false;
    }
    for (int i = 0; i < NUM_LEDS; i++) {
        LEDs[i] = blend(CRGB(0, 128, 128), CRGB(0, 128, 128), step);
    }
    FastLED.show();
    step += 1;
    return true;
}

void setup() {
    FastLED.addLeds<WS2812B, LED, GRB>(LEDs, NUM_LEDS);
    FastLED.setBrightness(255);
    FastLED.clear();
    FastLED.show();
}

void loop() {
    fadeLED();
    delay(10);
}