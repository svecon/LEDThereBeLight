#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_LEDS 390
#define PI_NUMS 135
#define BRIGHTNESS 25
#define PIN_NUMBER 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

bool charstack[] = {true, true, true, false, true, false, true, true, true, true, false, true, false, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, false, true, true, false, true, true, true, true, true, true, false, true, true, true, true, true, false, true, true, true, false, true, true, true, true, true, false, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, false, true, true, true, false, true, true, false, true, true, true, false, true, true, true, true, true, true, true, true, false, true, true, true, true, false, true, true, true, true, true, true, false, true, true, false, true, true, true, true, true, true, false, true};
bool leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  initStrip(&strip);
}

void initStrip(Adafruit_NeoPixel* strip) {
  strip->setBrightness(BRIGHTNESS);
  strip->begin();
  strip->show();
}

void loop() {
  piMorse(&strip, 150);
}

void piMorse(Adafruit_NeoPixel* strip, uint16_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    leds[i] = false;
  }

  int currentLetter = 0;
  int charstackUsed = PI_NUMS;
  int currentCharstack = 0;
  int zeroes = 0;
  int looping = strip->numPixels();
  while(looping > 0) {
    // when all morse is used, allow for snake tail to reach end
    if(currentCharstack >= charstackUsed){
      looping--;
    }

    // move all by one
    for(uint16_t i=strip->numPixels()-1; i>=1; i--) {
      leds[i] = leds[i-1];
    }

    if (currentCharstack < charstackUsed) {
        leds[0] = charstack[currentCharstack];
        if((leds[0]==false) && (zeroes<3-1)) {
          zeroes++;
        } else {
          currentCharstack++;
          zeroes = 0;
        }
    }
    else {
        leds[0] = false;
    }

    // set colors
    for(int i=0; i<strip->numPixels(); i++) {
      if(leds[i]) {
        strip->setPixelColor(i, Wheel(strip, ((i * 256 / strip->numPixels()) + currentLetter) & 255));
      } else {
        strip->setPixelColor(i, strip->Color(0,0,0));
      }
    }
    strip->show();
    delay(wait);
  }
}

void solidColor(Adafruit_NeoPixel* strip, uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
  }
  strip->show();
  delay(wait);
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel* strip, uint32_t c, uint16_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
    strip->show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel* strip, uint16_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(strip, ((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

void rainbow(Adafruit_NeoPixel* strip, uint16_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel* strip, byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}
