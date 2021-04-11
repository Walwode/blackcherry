#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS          29
#define DATA_PIN          7
#define BRIGHTNESS        100
#define FRAMES_PER_SECOND 60

#define REED_POWER_PIN    8
#define REED_READ_PIN     5
// #define MOSFET_GATE_PIN   3

CRGB leds[NUM_LEDS];
uint8_t gHue = 0;
void fadeWhite();
void solidWhite();
void sinelon();
uint8_t open = 0;
int currentLed = 0;
int maxLed = NUM_LEDS / 2;

void setup() {

  Serial.begin(115200);

  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  pinMode(DATA_PIN, OUTPUT);
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness( BRIGHTNESS );
  // pinMode(MOSFET_GATE_PIN, OUTPUT);
  // digitalWrite(MOSFET_GATE_PIN, HIGH);

  pinMode(REED_POWER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(REED_POWER_PIN, HIGH);
  open = digitalRead(REED_READ_PIN);
  digitalWrite(REED_POWER_PIN, LOW);

  if (!open) {
    FastLED.setBrightness(BRIGHTNESS);
    // sinelon();
    fadeWhite();
    FastLED.show();
    delay(1000 / FRAMES_PER_SECOND);
    // EVERY_N_MILLISECONDS( 20 ) { gHue++; }

  } else {
    Serial.println("Doors closed");
    currentLed = 0;
    uint8_t luma = leds[maxLed].getLuma();
    if (luma == 0) {
      delay(500);
    } else {
      fadeToBlackBy( leds, NUM_LEDS, 20);
      // FastLED.setBrightness(0);
      // fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(1000 / FRAMES_PER_SECOND);
    }
  }
}

void fadeWhite() {
  leds[maxLed] = CRGB::White;
  if (currentLed < maxLed) {
    currentLed += 1;
    Serial.print(currentLed);
    Serial.print("/");
    Serial.println(maxLed);
    leds[maxLed + currentLed] = CRGB::White;
    leds[maxLed - currentLed] = CRGB::White;
  }
}

void solidWhite() {
  fill_solid(leds, NUM_LEDS, CRGB::White);
}

void sinelon() {
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 30, 0, NUM_LEDS - 1 );
  // leds[pos] = CRGB::White;
  leds[pos] += CHSV( gHue, 255, 192);
}