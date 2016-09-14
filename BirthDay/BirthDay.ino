#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_LEDS 390
#define BRIGHTNESS 25
#define PIN_NUMBER 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_NUMBER, NEO_GRB + NEO_KHZ800);

// http://morsecode.scphillips.com/translator.html
String morse = ".... .- .--. .--. -.-- / -... .. .-. - .... -.. .- -.-- --..-- / -.-. .- - / ..-. ..-";

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
  pulsingMorse(&strip, morse, 100);
  snakeMorse(&strip, morse, 50);
}

void pulsingMorse(Adafruit_NeoPixel* strip, String morse, uint8_t wait) {
  for(int i = 0; i < morse.length(); i++){
    strip->setBrightness(BRIGHTNESS);
    for(uint16_t j=0; j<strip->numPixels(); j++) {
      strip->setPixelColor(j, Wheel(strip, ((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    
    if(morse[i] == '.') {
      delay(wait);
    } else if(morse[i] == '-') {
      delay(wait*4);
    } else if(morse[i] == '/') {
      delay(wait);
    } else if(morse[i] == ' ') {
      delay(wait*3);
    }

    // fade out
    for(int j=10; j>=0; j--) {
      strip->setBrightness(BRIGHTNESS*(10-j)/10);
      strip->show();
      delay(wait/10);
    }
    // turn off
    for(uint16_t j=0; j<strip->numPixels(); j++) {
      strip->setPixelColor(j, strip->Color(0,0,0));
    }
    strip->show();
    delay(wait);
  }
}

void snakeMorse(Adafruit_NeoPixel* strip, String morse, uint8_t wait) {
  bool charstack[4];
  bool leds[strip->numPixels()];
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    leds[i] = false;
  }

  int currentLetter = 0;
  int charstackUsed = 0;
  int currentCharstack = 0;
  int looping = strip->numPixels();
  while(looping > 0) {
    // when all morse is used, allow for snake tail to reach end
    if(currentLetter >= morse.length()){
      looping--; 
    }

    // move all by one
    for(uint16_t i=strip->numPixels()-1; i>=1; i--) {
      leds[i] = leds[i-1];
    }

    // determine next pixel
    if(currentCharstack >= charstackUsed && currentLetter < morse.length()) {
      if(morse[currentLetter] == '.') {
        charstack[0] = true;
        charstack[1] = false;
        charstackUsed = 2;
      } else if(morse[currentLetter] == '-') {
        charstack[0] = true;
        charstack[1] = true;
        charstack[2] = true;
        charstack[3] = false;
        charstackUsed = 4;
      } else if(morse[currentLetter] == ' ') {
        charstack[0] = false;
        charstack[1] = false;
        charstack[2] = false;
        charstackUsed = 3;
      } else if(morse[currentLetter] == '/') {
        charstack[0] = false;
        charstackUsed = 1;
      }
      currentLetter++;
      currentCharstack = 0;
    }
    leds[0] = charstack[currentCharstack++];

    // set colors
    for(int i=0; i<strip->numPixels(); i++) {
      if(leds[i]) {
        strip->setPixelColor(i, Wheel(strip, ((i * 256 / strip->numPixels())) & 255));
      } else {
        strip->setPixelColor(i, strip->Color(0,0,0));
      }
    }
    strip->show();
    delay(wait);
  }
}

void solidColor(Adafruit_NeoPixel* strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
  }
  strip->show();
  delay(wait);
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel* strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip->numPixels(); i++) {
    strip->setPixelColor(i, c);
    strip->show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel* strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip->numPixels(); i++) {
      strip->setPixelColor(i, Wheel(strip, ((i * 256 / strip->numPixels()) + j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

void rainbow(Adafruit_NeoPixel* strip, uint8_t wait) {
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


