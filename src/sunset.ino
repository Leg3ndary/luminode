/*
 * Modified for Ben Zhou's WS2812B LED strip with Pin 18
 *
 * Fill a gradient of colors and smoothly animate it.
 */

#include <FastLED.h> 

#define LED_PIN 18        // Data pin for WS2812B strip
#define NUM_LEDS 259      // Number of LEDs in your strip
#define COLOR_ORDER GRB   // Color order for WS2812B (typically GRB)
#define LED_TYPE WS2812B  // LED strip type

uint8_t max_bright = 250;

CRGB leds[NUM_LEDS];

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(
        5, 500);
    FastLED.setBrightness(max_bright);
}

void loop() {
    fill_grad();
    FastLED.show();
    delay(10);
}

void fill_grad() {
    uint8_t starthue =
        beatsin8(5, 0, 255);
    uint8_t endhue =
        beatsin8(7, 0, 255);

    if (starthue < endhue) {
        fill_gradient(leds, NUM_LEDS, CHSV(starthue, 255, 255),
                      CHSV(endhue, 255, 255), FORWARD_HUES);
    } else {
        fill_gradient(leds, NUM_LEDS, CHSV(starthue, 255, 255),
                      CHSV(endhue, 255, 255), BACKWARD_HUES);
    }
}
