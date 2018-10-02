#include "FastLED.h"

#define NUM_LEDS 18
#define DATA_PIN 9
CRGB leds[NUM_LEDS];

int currentLed = NUM_LEDS;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(96);
  pinMode(11, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, INPUT);
  
  for (int i = 0; i < NUM_LEDS; i++) { leds[i] = CRGB::Black; }
  FastLED.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(7, HIGH);
  // delay(100);
  if (digitalRead(5)) {
    switchOff();
    Serial.println("High!");
  } else {
    switchOn();
    Serial.println("Low");
  }
  digitalWrite(7, LOW);
  
  if (currentLed > -1) {
    delay(20);
  }
  else {
    delay(1000);
  }
  Serial.println(currentLed);
}

bool switchOn() {
  leds[currentLed] = CRGB::White;
  FastLED.show();
  if (currentLed < NUM_LEDS) digitalWrite(11, HIGH);
  if (currentLed > -1) { currentLed--; return true; }
}

bool switchOff() {
  leds[currentLed] = CRGB::Black;
  FastLED.show();
  if (currentLed == NUM_LEDS) digitalWrite(11, LOW);
  if (currentLed < NUM_LEDS) { currentLed++; return true; }
}

