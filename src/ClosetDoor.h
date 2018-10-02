#include <FastLED.h>
#include "LedStripe.h"

enum AnimationMode {
  AnimationModeIncrease,
  AnimationModeDecrease
};

class ClosetDoor : public LedStripe {

public:
  byte reetDataPin;
  byte reetVccPin;
  int currentLed;
  AnimationMode animationMode;
  
  ClosetDoor::ClosetDoor(CRGB& leds, byte vccPin, byte reetDataPin, byte reetVccPin, AnimationMode mode) : LedStripe(leds, vccPin) {
    
    pinMode(reetDataPin, INPUT);
    pinMode(reetVccPin, OUTPUT);
    
    this->animationMode = mode;
    switch (animationMode) {
      case AnimationModeIncrease:
        this->currentLed = 0;
        break;
      case AnimationModeDecrease:
        this->currentLed = ledCount - 1;
        break;
    }
  }
  ClosetDoor::~ClosetDoor() {};
  
  bool doorOpen() {
    digitalWrite(reetVccPin, HIGH);
    bool open = digitalRead(reetDataPin);
    digitalWrite(reetVccPin, LOW);
    return open;
  }
  
  void loop() {
    if (doorOpen()) nextAnimationOn();
    else nextAnimationOff();
  }
  
  void nextAnimationOn() {
    leds[currentLed] = CRGB::White;
    handleCurrentLed(true);
  }
  
  void nextAnimationOff() {
    leds[currentLed] = CRGB::Black;
    handleCurrentLed(false);
  }
  
  bool inAnimation() {
    return ((currentLed >= 0) || (currentLed <= (ledCount - 1)));
  }
  
  void handleCurrentLed(bool animationOn) {
    switch (animationMode) {
      case AnimationModeIncrease:
        if (animationOn) {  if (currentLed <= (ledCount - 1)) currentLed++; }
        else { if (currentLed >= 0) currentLed--; }
        break;
      case AnimationModeDecrease:
        if (animationOn) {  if (currentLed >= 0) currentLed--; }
        else { if (currentLed <= (ledCount - 1)) currentLed++; }
        break;
    }
  }
  
  // TODO: auto switchOff after interval

}; // class ClosetDoor

