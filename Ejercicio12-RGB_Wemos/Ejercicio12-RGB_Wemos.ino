#include <Adafruit_NeoPixel.h>

#define PIN   D4
#define LED_NUM 7

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

int velocidad = 200;

void setup() {
  leds.begin(); // This initializes the NeoPixel library.
}

void loop() {
  for (int j = 0; j < 5; j++) {
    for (int i = 1; i < LED_NUM; i++) {
      leds.setPixelColor(i - 1, leds.Color(0, 0, 0));
      leds.setPixelColor(i, leds.Color(10, 0, 0));
      leds.show();
      delay(velocidad);
    }
  }
  velocidad -= 20;
  if (velocidad < 20)
    velocidad = 200;
}
