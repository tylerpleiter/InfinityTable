#include <FastLED.h>
#include <time.h>

// Blue, Red, Black, White

FASTLED_USING_NAMESPACE

#define NUM_STRIPS  4
#define NUM_LEDS    147
CRGB leds[NUM_STRIPS][NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void setup() {
  delay(2000); // 3 second delay for recovery
  Serial.begin(115200);
  Serial.println("setup");

  // tell FastLED about the LED strip configuration
  FastLED.addLeds<WS2812B, 4, GRB>(leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 5, GRB>(leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 6, GRB>(leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812B, 7, GRB>(leds[3], NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setMaxPowerInVoltsAndMilliamps( 5, 10000);
  FastLED.setBrightness(BRIGHTNESS);

}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {solid, rainbow, rainbowGlitter, confetti, sinelon, juggle, bpm, noise, noiseSingle, waves, dripping};

uint8_t gCurrentPatternNumber = 1; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
char c;
String serialBufStr;
char serialBuf[10] = "";
long int hexColour = 13789440;
uint8_t serialInt;
uint8_t drips[NUM_LEDS];


void loop()
{
  // tell esp mega is ready for data
  Serial.println("OK");
  delay(10);
  if (Serial.available()) {
    serialBufStr = Serial.readStringUntil('\n');

    if (serialBufStr[0] == 'c') {
      serialBufStr = serialBufStr.substring(1, 7);
      serialBufStr.toCharArray(serialBuf, 7);
      hexColour = strtol(serialBuf, NULL, 16);
      gCurrentPatternNumber = 0;
    }

    if (serialBufStr[0] == 'p') {
      serialBufStr = serialBufStr.substring(1, 3);
      serialInt = serialBufStr.toInt();
      if (serialInt < ARRAY_SIZE(gPatterns) + 1) {
        gCurrentPatternNumber = serialInt;
      }
    }
  }

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
}


void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void solid() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[0][i] = hexColour;
  }
  extend();
}

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds[0], NUM_LEDS, gHue, 7);
  extend();
}

void rainbowGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
  extend();
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[0][ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds[0], NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[0][pos] += CHSV( gHue + random8(64), 200, 255);
  extend();
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds[0], NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[0][pos] += CHSV( gHue, 255, 192);
  extend();
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[0][i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  extend();
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds[0], NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[0][beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  extend();
}

void noise() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[i][j].setRGB(random8(), random8(), random8());
    }
  }
}

void noiseSingle() {
  CHSV baseColour = rgb2hsv_approximate(hexColour);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < NUM_LEDS; j++) {
      leds[i][j].setHSV(baseColour.hue, baseColour.sat + random8(50) - 25, baseColour.val + random8(50) - 25);
    }
  }
}

void waves() {
  CHSV baseColour = rgb2hsv_approximate(hexColour);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[0][i].setHSV(baseColour.hue, baseColour.sat - 100 * sin(0.1282 * (i + gHue)) - 100, baseColour.val);
  }
  extend();
}


void dripping() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[0][i].setRGB(0, 0, 255);

    if (random8() < 2) {
      drips[i] = 100;
    }

    leds[3][i] = 0;
    leds[2][i] = 0;
    leds[1][i] = 0;
    
    if (drips[i] > 50) {
      leds[1][i].setRGB(0, 0, 255);
      drips[i]--;
    } else if (drips[i] > 10) {
      leds[2][i].setRGB(0, 0, 255);
      leds[1][i].setRGB(0, 0, 255);
      drips[i]--;
    } else if (drips[i] > 0) {
      leds[3][i].setRGB(0, 0, 255);
      leds[2][i].setRGB(0, 0, 255);
      leds[1][i].setRGB(0, 0, 255);
      drips[i]--;
    }

  }
}

void extend() {
  memcpy(leds[1], leds[0], sizeof(leds[0]));
  memcpy(leds[2], leds[0], sizeof(leds[0]));
  memcpy(leds[3], leds[0], sizeof(leds[0]));
}
