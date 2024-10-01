// /*
// Created by: Ben Zhou
// */

// #define LED 18
// #define NUM_LEDS 259
// #include <FastLED.h>

// CRGB LEDs[NUM_LEDS];

// int step = 0;

// bool fadeLED() {
    
//     if (step > 255) {
//         step = 0;
//         return false;
//     }
//     for (int i = 0; i < NUM_LEDS; i++) {
//         LEDs[i] = blend(CRGB(0, 128, 128), CRGB(0, 128, 128), step);
//     }
//     FastLED.show();
//     step += 1;
//     return true;
// }

// void setup() {
//     FastLED.addLeds<WS2812B, LED, GRB>(LEDs, NUM_LEDS);
//     FastLED.setBrightness(255);
//     FastLED.clear();
//     FastLED.show();
// }

// void loop() {
//     fadeLED();
//     delay(10);
// }


/*
Created by: Ben Zhou
*/

#define LED 18
#define NUM_LEDS 259
#include <FastLED.h>

CRGB LEDs[NUM_LEDS];

int step = 0;

bool sunsetFade() {
    
    if (step > 255) {
        step = 0;
    }

    CRGB startColor;
    CRGB endColor;

    // Transition between red -> orange -> yellow -> back to red
    if (step <= 85) {
        // Red to Orange
        startColor = CRGB::Red;
        endColor = CRGB::Orange;
        for (int i = 0; i < NUM_LEDS; i++) {
            LEDs[i] = blend(startColor, endColor, (step * 3));
        }
    } else if (step > 85 && step <= 170) {
        // Orange to Yellow
        startColor = CRGB::Orange;
        endColor = CRGB::Yellow;
        for (int i = 0; i < NUM_LEDS; i++) {
            LEDs[i] = blend(startColor, endColor, ((step - 85) * 3));
        }
    } else {
        // Yellow back to Red
        startColor = CRGB::Yellow;
        endColor = CRGB::Red;
        for (int i = 0; i < NUM_LEDS; i++) {
            LEDs[i] = blend(startColor, endColor, ((step - 170) * 3));
        }
    }

    FastLED.show();
    step += 1;

    if (step >= 256) {
        step = 0;
    }

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
