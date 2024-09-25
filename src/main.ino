/*
Created by: Ben Zhou
*/

#define LED 18
#define NUM_LEDS 60

#include <FastLED.h>

CRGB LEDs[NUM_LEDS];

void setup() {
    FastLED.addLeds<WS2812B, LED, GRB>(LEDs, NUM_LEDS);
    FastLED.setBrightness(255);
    FastLED.clear();
    FastLED.show();
}

void loop() {

}