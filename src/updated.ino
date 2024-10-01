/*
 * Modified for Ben Zhou's WS2812B LED strip with Pin 18
 *
 * Fill a gradient of colors and smoothly animate it when Spotify is not playing.
 * Fade to the current Spotify song's color when music is playing.
 */

#include <FastLED.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Ticker.h>

// LED strip configuration
#define LED_PIN 18        // Data pin for WS2812B strip
#define NUM_LEDS 259      // Number of LEDs in your strip
#define COLOR_ORDER GRB   // Color order for WS2812B (typically GRB)
#define LED_TYPE WS2812B  // LED strip type

uint8_t max_bright = 250;
CRGB leds[NUM_LEDS];

WiFiClientSecure wifiClient;
Ticker ledTicker;
Ticker spotifyTicker;

// Wi-Fi credentials
const char* ssid = "eduroam";
const char* password = "";

// Server and API credentials
const char* server = "bzhou.ca";
const char* api_password = "your_API_password";
const char* benzServerCert = "your_certificate_here";  // Add your server certificate

bool isPlaying = false; // To track if music is playing

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    FastLED.setBrightness(max_bright);

    // Initialize Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
    wifiClient.setCACert(benzServerCert);

    // Initialize task scheduling
    ledTicker.attach(0.003, updateLEDs);  // Update LEDs every 3ms
    spotifyTicker.attach(5, checkSpotifyStatus);  // Check Spotify status every 5 seconds
}

void loop() {
    // FastLED.show() will be called within updateLEDs() task
}

// Task to update LEDs
void updateLEDs() {
    if (isPlaying) {
        fadeToSpotifyColor();
    } else {
        fill_grad();
    }
    FastLED.show();
}

void fill_grad() {
    uint8_t starthue = beatsin8(5, 0, 255);
    uint8_t endhue = beatsin8(7, 0, 255);

    if (starthue < endhue) {
        fill_gradient(leds, NUM_LEDS, CHSV(starthue, 255, 255),
                      CHSV(endhue, 255, 255), FORWARD_HUES);
    } else {
        fill_gradient(leds, NUM_LEDS, CHSV(starthue, 255, 255),
                      CHSV(endhue, 255, 255), BACKWARD_HUES);
    }
}

void checkSpotifyStatus() {
    if (!wifiClient.connected()) {
        if (!wifiClient.connect(server, 443)) {
            isPlaying = false;
            return;
        }
    }

    wifiClient.print(String("GET /api/getCurrent/") + api_password + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: Keep-Alive\r\n\r\n");

    // Wait for response
    unsigned long timeout = millis();
    while (!wifiClient.available()) {
        if (millis() - timeout > 5000) {
            isPlaying = false;
            return;
        }
    }

    String response = "";
    while (wifiClient.available()) {
        response += char(wifiClient.read());
    }

    if (response.indexOf("\"paused\":false") != -1) {
        isPlaying = true;
    } else {
        isPlaying = false;
    }
}

void fadeToSpotifyColor() {
    if (!wifiClient.connected()) {
        if (!wifiClient.connect(server, 443)) {
            return;
        }
    }

    wifiClient.print(String("GET /api/getCurrent/") + api_password + " HTTP/1.1\r\n" +
                     "Host: " + server + "\r\n" +
                     "Connection: Keep-Alive\r\n\r\n");

    // Wait for response
    unsigned long timeout = millis();
    while (!wifiClient.available()) {
        if (millis() - timeout > 5000) {
            return;
        }
    }

    String response = "";
    while (wifiClient.available()) {
        response += char(wifiClient.read());
    }

    String color = response.substring(response.indexOf("[") + 1, response.indexOf("]"));
    int comma1 = color.indexOf(",");
    int comma2 = color.indexOf(",", comma1 + 1);

    int red = color.substring(0, comma1).toInt();
    int green = color.substring(comma1 + 1, comma2).toInt();
    int blue = color.substring(comma2 + 1).toInt();

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(red, green, blue);
    }
}
