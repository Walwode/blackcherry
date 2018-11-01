#include "FastLED.h"

#define NUM_LEDS 5
#define PIN_SHIFT_SHIFT 4
#define PIN_SHIFT_STORE 2
#define PIN_SHIFT_DATA  3
#define PIN_REET_INPUT  5

CRGB leds[2][3][NUM_LEDS];

int shiftRegister[8] = {1, 1, 0, 0, 0, 0, 0, 0}; // 1, 2, 13, 12, 11, 10, 9, 8

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, 13>(leds[0][0], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 12>(leds[0][1], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 11>(leds[0][2], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 10>(leds[1][0], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL,  9>(leds[1][1], NUM_LEDS);
  FastLED.addLeds<NEOPIXEL,  8>(leds[1][2], NUM_LEDS);
  FastLED.setBrightness(200);

  pinMode(PIN_SHIFT_STORE, OUTPUT);
  pinMode(PIN_SHIFT_SHIFT, OUTPUT);
  pinMode(PIN_SHIFT_DATA, OUTPUT);
  pinMode(PIN_REET_INPUT, INPUT);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      fill_solid(leds[i][j], NUM_LEDS, CRGB::Black);
    }
  }
  FastLED.show();

  Serial.println("Setup done");
}

void loop() {
  byte doorClosed[6] = {0, 0, 0, 0, 0, 0};
  
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      shiftRegister[(i * 3) + j + 2] = 1;
      setShiftRegister();
      
      doorClosed[(i * 3) + j] = digitalRead(PIN_REET_INPUT);
      if (doorClosed[(i * 3) + j]) switchOff(i, j);
      else switchOn(i, j);
      
      shiftRegister[(i * 3) + j + 2] = 0;
      setShiftRegister();
    }
  }
  
  shiftRegister[0] = !doorClosed[0] || !doorClosed[1] || !doorClosed[2];
  shiftRegister[1] = !doorClosed[3] || !doorClosed[4] || !doorClosed[5];
  setShiftRegister();
  
  FastLED.show();
  delay(200);
}

void switchOn(int i, int j) {
  fill_solid(leds[i][j], NUM_LEDS, CRGB(255,230,200));
  // fill_solid(leds[i][j], NUM_LEDS, CRGB::White);
}

void switchOff(int i, int j) {
  fill_solid(leds[i][j], NUM_LEDS, CRGB::Black);
}

void setShiftRegister() {
  digitalWrite(PIN_SHIFT_STORE, LOW);
  for (int i = 0; i < 8; i++) {
    digitalWrite(PIN_SHIFT_SHIFT, LOW);
    digitalWrite(PIN_SHIFT_DATA, shiftRegister[i]);
    digitalWrite(PIN_SHIFT_SHIFT, HIGH);
  }
  digitalWrite(PIN_SHIFT_STORE, HIGH);
}

