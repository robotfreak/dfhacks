/* Digital Fotografie Hacks
 * LightPainting: 
 * Lichtmalerie mit Adafruit Neopixel Sticks
 * v0.1, 2014-08-18, Peter Recktenwald 
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/ 
#include <Adafruit_NeoPixel.h>

/*-----( Definitionen )-----*/ 
#define pixelPin 7
#define btn1Pin  3
#define btn2Pin  4
#define btn3Pin  A2

#define BUTTON1  1
#define BUTTON2  2
#define BUTTON3  4

#define BRIGHTNESS_STEPS 5

enum eMode {
  MOD_INIT,
  MOD_SOLID,
  MOD_STRIPE,
  MOD_RAINBOW,
  MOD_LAST
};

enum eColorMode {
  COL_RED,
  COL_GREEN,
  COL_BLUE,
  COL_WHITE,
  COL_LAST   
};   

/*-----( Variablen )-----*/ 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, pixelPin, NEO_GRB + NEO_KHZ800);

int mode = MOD_INIT;
int colorMode = COL_RED;
int brightnessIdx = BRIGHTNESS_STEPS-1;
int brightness;

int brightnessTbl[BRIGHTNESS_STEPS] = {32, 64, 128, 192, 255};

void initButtons(void)
{
  // Button pins as inputs 
  pinMode(btn1Pin, INPUT);
  pinMode(btn2Pin, INPUT);
  pinMode(btn3Pin, INPUT);
  // enable internal PullUp
  digitalWrite(btn1Pin, HIGH);
  digitalWrite(btn2Pin, HIGH);
  digitalWrite(btn3Pin, HIGH);
}

int readButtons(void)
{
  int result = 0;
  
  if (digitalRead(btn1Pin) == 0)
  {
    while (digitalRead(btn1Pin) == 0);
    result |= BUTTON1;
  }
  if (digitalRead(btn2Pin) == 0)
  {
    while (digitalRead(btn2Pin) == 0);
    result |= BUTTON2;
  }
  if (digitalRead(btn3Pin) == 0)
  {
    while (digitalRead(btn3Pin) == 0);
    result |= BUTTON3;
  }
  return result;    
}
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  initButtons();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  int btn;
  btn = readButtons();
  
  if (btn & BUTTON1)  // Mode
  {
    if (mode < (MOD_LAST-1)) mode++; else mode = MOD_INIT;
  }
  if (btn & BUTTON2)  // Helligkeit
  {
    if (brightnessIdx < (BRIGHTNESS_STEPS-1)) brightnessIdx++; else brightnessIdx = 0;
  }
  if (btn & BUTTON3)  // Farbe
  {
    if (colorMode < (COL_LAST-1)) colorMode++; else colorMode = COL_RED;
  } 
  
  brightness = brightnessTbl[brightnessIdx];
  switch (mode) 
  {
    case MOD_SOLID:
      switch (colorMode)
      {
        case COL_RED:
          colorWipe(strip.Color(brightness, 0, 0)); // Red
        break;
        case COL_GREEN:
          colorWipe(strip.Color(0, brightness, 0)); // Green
        break;
        case COL_BLUE:
          colorWipe(strip.Color(0, 0, brightness)); // Blue
        break;
        case COL_WHITE:
          colorWipe(strip.Color(brightness, brightness, brightness)); // White
        break;
      }
    break;
    case MOD_STRIPE:
      switch (colorMode)
      {
        case COL_RED:
          colorStripe(strip.Color(brightness, 0, 0)); // Red
        break;
        case COL_GREEN:
          colorStripe(strip.Color(0, brightness, 0)); // Green
        break;
        case COL_BLUE:
          colorStripe(strip.Color(0, 0, brightness)); // Blue
        break;
        case COL_WHITE:
          colorStripe(strip.Color(brightness, brightness, brightness)); // White
        break;
      }
    break;
    case MOD_RAINBOW:
      rainbow();
    break;
    default:
      colorWipe(strip.Color(0, 0, 0));
    break;
  }
    

//  rainbow(20);
//  rainbowCycle(20);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}


// Fill the dots one after the other with a color
void colorStripe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      if (i&1)
        strip.setPixelColor(i, c);
      else
        strip.setPixelColor(i, 0);
      strip.show();
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbow(void) {
  uint16_t i;

  for(i=0; i< strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels())) & 255));
  }
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

