/* Digital Fotografie Hacks
 * Neopixel POV: 
 * Ein POV Display mit Adafruit Neopixel Sticks
 * v0.1, 2014-12-14, Peter Recktenwald 
*/
/*-----( Importiere benoetigte Bibliotheken )-----*/ 
#include <Adafruit_NeoPixel.h>
#include "kerze888.c"

/*-----( Definitionen )-----*/ 
#define pixelPin 7
#define btn1Pin  3
#define btn2Pin  4
#define btn3Pin  A2

#define BUTTON1  1
#define BUTTON2  2
#define BUTTON3  4

#define BRIGHTNESS_STEPS 5

#define COLUMN_TIME  15 // Time[ms] of every single column is switched on
#define SPACER       10 // This value multiplied the columnTime to get a spacer between the content of the whole array
#define COLUMN_COUNT  9  //Counting columns

int col_r[COLUMN_COUNT] = { 0x0000, 0x0000, 0x07FC, 0x27FC, 0xFFFC, 0x27FC, 0x07FC, 0x0000, 0x0000 }; 
int col_g[COLUMN_COUNT] = { 0x0003, 0x0007, 0x0003, 0x2003, 0x7803, 0x2003, 0x0003, 0x0007, 0x0003 }; 
int col_b[COLUMN_COUNT] = { 0x0000, 0x0000, 0x0000, 0x2000, 0x7800, 0x2000, 0x0000, 0x0000, 0x0000 }; 

 
/*-----( Variablen )-----*/ 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, pixelPin, NEO_GRB + NEO_KHZ800);

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
  
  if (btn & BUTTON2)  // Helligkeit
  {
    if (brightnessIdx < (BRIGHTNESS_STEPS-1)) brightnessIdx++; else brightnessIdx = 0;
  }
  
  brightness = brightnessTbl[brightnessIdx];
  bitmap32POV(); 
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
  }
}


// colorPOV
void colorPOV(void) 
{
  int r,g,b;
  for (uint16_t column=0; column<COLUMN_COUNT; column++)
  {
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
        if (bitRead(col_r[column],i)) r = brightness; else r = 0;
        if (bitRead(col_g[column],i)) g = brightness; else g = 0;
        if (bitRead(col_b[column],i)) b = brightness; else b = 0;
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
    delay(COLUMN_TIME );
  }
//  colorWipe(0);
  delay(SPACER*COLUMN_TIME);
}

// bitmap32POV
void bitmap32POV(void) 
{
  int r,g,b;
  int bmpcnt = 0;
  for (uint16_t column=0; column<BMPHEIGHT; column++)
  {
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
        r = (image32[bmpcnt] & 0x00ff0000) >> 16;
        g = (image32[bmpcnt] & 0x0000ff00) >> 8;
        b = (image32[bmpcnt] & 0x000000ff);
        strip.setPixelColor(i, strip.Color(r, g, b));
    }
    strip.show();
    delay(COLUMN_TIME );
  }
  colorWipe(0);
  delay(SPACER*COLUMN_TIME);
}

