#include "led_animation.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void initLEDs() {
    strip.begin();
    strip.show();
}

// Animation finale
void ledDance() {
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0)); // Rouge
        strip.show();
        delay(100);
    }
    for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 255)); // Bleu
        strip.show();
        delay(100);
    }
}
