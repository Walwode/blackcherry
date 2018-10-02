#include "LowPower.h"
#include "FastLED.h"

#define NUM_LEDS 12
#define DATA_PIN_RIGHT 9
#define DATA_PIN_LEFT 8
CRGB leds_right[NUM_LEDS];
CRGB leds_left[NUM_LEDS];

#define SENSOR_PIN_LEFT 5
#define SENSOR_PIN_RIGHT 4

#define BRIGHTNESS         10
#define FRAMES_PER_SECOND  120

int currLeftPixel = 0;
int currRightPixel = 0;

void setup() {
  Serial.begin(9600);
  setupLed();
  setupSensor();
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds_right[i] = CRGB::White;
  }
  FastLED.show();
  
  for (int i = 0; i < 6; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  
}

void loop() {
  Serial.println(digitalRead(SENSOR_PIN_RIGHT));

  digitalWrite(LED_BUILTIN, HIGH);
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Power down for 8 seconds...");
  delay(100);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  Serial.println("Welcome back...");
  
  // LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
  //               SPI_OFF, USART0_OFF, TWI_OFF);  

  /*
  bool updateLed;
  updateLed = updateLed || loopRightDoor();
  updateLed = updateLed || loopLeftDoor();
  if (updateLed) {
    FastLED.show();
    // FastLED.delay(1000/FRAMES_PER_SECOND);
    delay(1000/FRAMES_PER_SECOND);
  } else { delay(1000); }
  */
}

void setupLed() {
  FastLED.setBrightness(BRIGHTNESS);
  
  FastLED.addLeds<WS2812B, DATA_PIN_RIGHT>(leds_right, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_LEFT>(leds_left, NUM_LEDS);
}

void setupSensor() {
  pinMode(SENSOR_PIN_LEFT,INPUT);
  pinMode(SENSOR_PIN_RIGHT,INPUT);
}

bool loopRightDoor() {
  if (!digitalRead(SENSOR_PIN_RIGHT)) {
    if (currRightPixel < NUM_LEDS) {
      leds_right[currRightPixel] = CRGB::White;
      currRightPixel++;
      return true;
    }
  } else if (currRightPixel > 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds_right[i] = CRGB::Black;
    }
    currRightPixel = 0;
    return true;
  }
  return false;
}

bool loopLeftDoor() {
  if (!digitalRead(SENSOR_PIN_LEFT)) {
    if (currLeftPixel < NUM_LEDS) {
      leds_left[currLeftPixel] = CRGB::White;
      currLeftPixel++;
      return true;
    }
  } else if (currLeftPixel > 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds_left[i] = CRGB::Black;
    }
    currLeftPixel = 0;
    return true;
  }
  return false;
}

int lpDelay(int quarterSeconds) {
  int oldClkPr = CLKPR;  // save old system clock prescale
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = 0x08;    // 1/256 prescaler = 60KHz for a 16MHz crystal
  delay(quarterSeconds);  // since the clock is slowed way down, delay(n) now acts like delay(n*256)
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = oldClkPr;    // Restore old system clock prescale
}

int lpDelayWithSleep() {
  int oldClkPr = CLKPR;  // save old system clock prescale
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = 0x08;    // 1/256 prescaler = 60KHz for a 16MHz crystal
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  CLKPR = 0x80;    // Tell the AtMega we want to change the system clock
  CLKPR = oldClkPr;    // Restore old system clock prescale
}

