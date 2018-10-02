#include "FastLED.h"

#define NUM_LEDS 47
#define DATA_PIN_RIGHT_TOP 5
#define DATA_PIN_RIGHT_MIDDLE 6
#define DATA_PIN_RIGHT_BOTTOM 7
#define DATA_PIN_LEFT 1
CRGB leds_right_top[NUM_LEDS];
CRGB leds_right_middle[NUM_LEDS];
CRGB leds_right_bottom[NUM_LEDS];
CRGB leds_left[NUM_LEDS];

#define SENSOR_PIN_LEFT 4
#define SENSOR_PIN_RIGHT 15

#define BRIGHTNESS         150
#define FRAMES_PER_SECOND  120

int currLeftPixel = 0;
int currRightPixel = 0;

void setup() {
  Serial.begin(115200);
  setupLed();
  setupSensor();
  Serial.println("Start...");
}

void loop() {
  loopRightDoor();
  loopLeftDoor();
  // delay(1000);
  
  FastLED.show();
  // FastLED.delay(1000/FRAMES_PER_SECOND);
  // Serial.println("Loop");
}

void setupLed() {
  FastLED.setBrightness(BRIGHTNESS);
  
  FastLED.addLeds<WS2812B, DATA_PIN_RIGHT_TOP>(leds_right_top, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_RIGHT_MIDDLE>(leds_right_middle, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_RIGHT_BOTTOM>(leds_right_bottom, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_LEFT>(leds_left, NUM_LEDS);
}

void setupSensor() {
  pinMode(SENSOR_PIN_LEFT,INPUT);
  pinMode(SENSOR_PIN_RIGHT,INPUT);
}

void loopRightDoor() {
  if (!digitalRead(SENSOR_PIN_RIGHT)) {
    Serial.println("Right On");
    if (currRightPixel < (NUM_LEDS * 3)) {
      int stripe = currRightPixel / NUM_LEDS;
      // Serial.println(currRightPixel);
      switch (stripe) {
        case 0:
          leds_right_top[currRightPixel % NUM_LEDS] = CRGB::White;
          break;
        case 1:
          leds_right_middle[NUM_LEDS - (currRightPixel % NUM_LEDS)] = CRGB::White;
          break;
        case 2:
          leds_right_bottom[currRightPixel % NUM_LEDS] = CRGB::White;
          break;
      }
      currRightPixel++;
      delay(10);
    }
  } else if (currRightPixel > 0) {
    Serial.println("Right Off");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds_right_top[i] = CRGB::Black;
      leds_right_middle[i] = CRGB::Black;
      leds_right_bottom[i] = CRGB::Black;
    }
    currRightPixel = 0;
  }
}

void loopLeftDoor() {
  if (!digitalRead(SENSOR_PIN_LEFT)) {
    Serial.println("Left On");
    if (currLeftPixel < NUM_LEDS) {
      leds_left[currLeftPixel] = CRGB::White;
      currLeftPixel++;
    }
  } else if (currLeftPixel > 0) {
    Serial.println("Left Off");
    for (int i = 0; i < NUM_LEDS; i++) {
      leds_left[i] = CRGB::Black;
    }
    currLeftPixel = 0;
  }
}

