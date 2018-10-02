#include <FastLED.h>

class LedStripe {
  
public:
  int ledCount;
  byte vccPin;

  CRGB* leds;
  
  LedStripe::LedStripe(CRGB& leds, byte vccPin) {
    this->leds = &leds;
    this->ledCount = sizeof(this->leds)/sizeof(CRGB);
    this->vccPin = vccPin;
    
    pinMode(vccPin, OUTPUT);
  };
  LedStripe::~LedStripe() {};
  
  void black() {
    fill_solid(leds, ledCount, CRGB::Black);
    FastLED.show();
  };
  
  void white() {
    fill_solid(leds, ledCount, CRGB::White);
    FastLED.show();
  };
}; // class LedStripe

