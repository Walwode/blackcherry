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
  
  ClosetDoor::ClosetDoor(CRGB * leds, int ledCount, byte vccPinLed, byte reetDataPin, byte reetVccPin, AnimationMode mode) : LedStripe(leds, ledCount, vccPinLed) {
    
    this->reetVccPin = reetVccPin;
    this->reetDataPin = reetDataPin;
    this->animationMode = mode;
    
    pinMode(this->reetDataPin, INPUT);
    pinMode(this->reetVccPin, OUTPUT);
    
    this->animationMode = mode;
    switch (this->animationMode) {
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
    bool open = !digitalRead(reetDataPin);
    digitalWrite(reetVccPin, LOW);
    return open;
  }
  
  void loop() {
    if (doorOpen()) nextAnimationOn();
    else nextAnimationOff();
  }
  
  void nextAnimationOn() {
    Serial.println("Animation on");
    leds[currentLed] = CRGB::White;
    switch (animationMode) {
      case AnimationModeIncrease:
        if (currentLed <= (ledCount - 1)) currentLed++;
        break;
      case AnimationModeDecrease:
        if (currentLed >= 0) currentLed--;
        break;
    }
  }
  
  void nextAnimationOff() {
    Serial.println("Animation off");
    leds[currentLed] = CRGB::Black;
    switch (animationMode) {
      case AnimationModeIncrease:
        if (currentLed >= 0) currentLed--;
        break;
      case AnimationModeDecrease:
        if (currentLed <= (ledCount - 1)) currentLed++;
        break;
    }
  }
  
  bool inAnimation() {
    Serial.println(currentLed);
    // return ((currentLed >= 0) && (currentLed <= (ledCount - 1)));
    return true;
  }
  
  // TODO: auto switchOff after interval

}; // class ClosetDoor

