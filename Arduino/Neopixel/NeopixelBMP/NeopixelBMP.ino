/* Digital Fotografie Hacks
 * Neopixel POV:
 * Ein POV Display mit Adafruit Neopixel Sticks
 * v0.1, 2014-12-14, Peter Recktenwald
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/
#include <Adafruit_NeoPixel.h>
#include <avr/pgmspace.h>
#include "./gamma.h"
#include "./image888.c"

/*-----( Definitionen )-----*/
#define pixelPin 7

#define COLUMN_TIME  15 // Time[ms] of every single column is switched on
#define SPACER       10 // This value multiplied the columnTime to get a spacer between the content of the whole array


/*-----( Variablen )-----*/
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, pixelPin, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  bitmap32POV(10);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

// bitmap32POV
void bitmap32POV(int repeat)
{
  int raw_r, raw_g, raw_b;
  int corr_r, corr_g, corr_b;
  int bmpcnt;
  for (int i = 0; i < repeat; i++)
  {
    bmpcnt = 0;
    for (uint16_t column = 0; column < BMPHEIGHT; column++)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i++)
      {
        raw_b = (image32[bmpcnt] & 0x00ff0000) >> 16;
        corr_b = pgm_read_byte(&gamma[raw_b]);
        raw_g = (image32[bmpcnt] & 0x0000ff00) >> 8;
        corr_g = pgm_read_byte(&gamma[raw_g]);
        raw_r = (image32[bmpcnt] & 0x000000ff);
        corr_r = pgm_read_byte(&gamma[raw_r]);
        bmpcnt++;
        strip.setPixelColor(i, strip.Color(corr_r, corr_g, corr_b));
      }
      strip.show();
      delay(COLUMN_TIME );
    }
  }
  colorWipe(0);
  delay(10* SPACER * COLUMN_TIME);
}

