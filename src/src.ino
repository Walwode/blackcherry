#include "FastLED.h"

enum AnimationMode {
	AnimationModeIncrease,
	AnimationModeDecrease
};

class LedStripe {
	
public:
	int ledCount;
	int brightness;
	byte dataPin;
	byte vccPin;
	
	CRGB* leds;
	
	PlantCircuit::PlantCircuit(int ledCount, byte dataPin, byte vccPin) {
		this->ledCount = ledCount;
		this->dataPin = dataPin;
		this->vccPin = vccPin;
		this->leds = new CRGB[ledCount];
		this->brightness = brightness;
		
		FastLED.addLeds<NEOPIXEL, DATA_PIN>(this->leds, this->ledCount);
		FastLED.setBrightness(this->brightness);
		
		pinMode(vccPin, OUTPUT);
		pinMode(dataPin, OUTPUT);
	}
	PlantCircuit::~PlantCircuit() {};
	
	void black() {
		for (int i = 0; i < this->ledCount; i++) { this->leds[i] = CRGB::Black; }
		FastLED.show();
	}
	
	void white() {
		for (int i = 0; i < this->ledCount; i++) { this->leds[i] = CRGB::White; }
		FastLED.show();
	}
} // class LedStripe


class ClosetStripe : public LedStripe {

public:
	byte reetDataPin;
	byte reetVccPin;
	int currentLed;
	AnimationMode animationMode;
	
	void initSwitch(byte reetDataPin, byte reetVccPin, AnimationMode mode) {
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
				if (animationOn) {	if (currentLed <= (ledCount - 1)) currentLed++; }
				else { if (currentLed >= 0) currentLed--; }
				break;
			case AnimationModeDecrease:
				if (animationOn) {	if (currentLed >= 0) currentLed--; }
				else { if (currentLed <= (ledCount - 1)) currentLed++; }
				break;
		}
	}
	
	// TODO: auto switchOff after interval

} // class ClosetStripe



ClosetStripe* rightDoor;

void setup() {
	Serial.begin(115200);

	rightDoor = new ClosetStripe(18, PIN_9, PIN_11, 96);
	rightDoor->initSwitch(PIN_5, PIN_7, AnimationModeDecrease);
	rightDoor->black();
}

void loop() {
	rightDoor->loop();
	
	if (rightDoor->inAnimation()) delay(20);
	else delay(1000);
}
