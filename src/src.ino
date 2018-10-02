#include <FastLED.h>
#include "ClosetDoor.h"

CRGB rightDoorLeds[18];
ClosetDoor rightDoor(*rightDoorLeds, 11, 5, 7, AnimationModeDecrease);

void setup() {
  Serial.begin(115200);
  FastLED.setBrightness(96);

  pinMode(9, OUTPUT);
  FastLED.addLeds<NEOPIXEL, 9>(rightDoorLeds, sizeof(rightDoorLeds)/sizeof(CRGB));
  
  rightDoor.black();
}

void loop() {
  rightDoor.loop();
  
  if (rightDoor.inAnimation()) delay(20);
  else delay(1000);
}

