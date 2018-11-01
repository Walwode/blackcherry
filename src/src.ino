#include <FastLED.h>
#include "ClosetDoor.h"

const int rightDoorLedCount = 18;
CRGB rightDoorLeds[rightDoorLedCount];
ClosetDoor rightDoor(rightDoorLeds, rightDoorLedCount, 11, 5, 7, AnimationModeDecrease);

void setup() {
  Serial.begin(115200);
  FastLED.setBrightness(96);

  pinMode(9, OUTPUT);
  FastLED.addLeds<NEOPIXEL, 9>(rightDoorLeds, rightDoorLedCount);
  
  rightDoor.black();
}

void loop() {
  rightDoor.loop();

  if (rightDoor.inAnimation()) delay(200);
  else delay(1000);
  Serial.println(rightDoor.ledCount);
}

