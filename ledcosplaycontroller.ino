
/* 

         )  (                       
   (  ( /(  )\ )       (            
   )\ )\())(()/(       )\   ) (     
 (((_|(_)\  /(_))`  ) ((_| /( )\ )  
 )\___ ((_)(_))  /(/(  _ )(_)|()/(  
((/ __/ _ \/ __|((_)_\| ((_)_ )(_)) 
 | (_| (_) \__ \| '_ \) / _` | || | 
  \___\___/|___/| .__/|_\__,_|\_, | 
                |_| controller |__/  


COSplay LED modular controller

 hardware platform  : Adafruit Metro Mini v2
                      Pro Trinket Lipoly backpack
 LEDs               : LEDstrip Neopixel 60 LEDS/m - WS2812B
                      Flora Neopixel V2
 Power              : 3.7v - 1800 mAh LiPo or 3.7v - 4400 mAh LiPo
 Codebase           : Arduino (C/C++)

Switches-Buttons
- digital port 2    : button (<color> cable) - trigger
- digital port 3    : switch (blue cable) - switch pre-defined animations A1-A2-A1B1-A1B2-A2B1-A2B2 
- digital port 4    : switch (yellow cable) - switch pre-defined animations B1-B2-A1B1-A1B2-A2B1-A2B2

Connectos
 - digital port 6   : two connectors LED strip
 - digital port 7   : one connector LED strip or steady lights
 - digital port 8   : weapon trigger or other LED/sound function (aux) 

 (2024) JinjiroSan

 ledcosplay.ino : v1-0.1 (alpha) - refactor C0.0.0
*/

// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>

#define LED_PIN_C6     6  // two connectors for LEDstrips
#define LED_PIN_C7     7  // one connector for LEDstrips
#define LED_PIN_C8     8  // one connector for aux

// How many NeoPixels are attached to each pin
#define LED_COUNT_C6  10
#define LED_COUNT_C7  10
//#define LED_COUNT_C8  10  // uncomment only if LED connected

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS_C6 50 // Set BRIGHTNESS to about 1/5 (max = 255)
#define BRIGHTNESS_C7 10 // controls brightness for for steady white lights

#define BUTTON_PIN_T2 2
#define BUTTON_PIN_T3 3
#define BUTTON_PIN_T4 4

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip_C6(LED_COUNT_C6, LED_PIN_C6, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel strip_C7(LED_COUNT_C7, LED_PIN_C7, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

const int buttonPin_T2 = 2; // the number of the trigger push button
const int buttonPin_T3 = 3; // the number of the switch #3
const int buttonPin_T4 = 4; // the number of the switch #4

int buttonState_T2 = 0; // variable for the pushbutton #2 status and initial value of 0
int buttonState_T3 = 0; // variable for the pushbutton #3 status and initial value of 0
int buttonState_T4 = 0; // variable for the pushbutton #4 status and initial value of 0

void setup() {
  strip_C6.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip_C6.show();            // Turn OFF all pixels ASAP
  strip_C6.setBrightness(BRIGHTNESS_C6);

  strip_C7.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  for(uint16_t i = 0; i < strip_C7.numPixels(); i++) {
    strip_C7.setPixelColor(i, 255, 100, 0);
  }
  //strip_C7.fill(strip_C7.Color(0, 0, 0, BRIGHTNESS_C7)); // turn on white LEDs to brightness level
  strip_C7.show();            // show the settings above
  strip_C7.setBrightness(BRIGHTNESS_C7);

  pinMode(buttonPin_T2, INPUT_PULLUP);
  pinMode(buttonPin_T3, INPUT_PULLUP);
  pinMode(buttonPin_T4, INPUT_PULLUP);

}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip_C6.numPixels(); i++) { // For each pixel in strip...
    strip_C6.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip_C6.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// strip_C7 pulsing brighter
void brighten() {
  uint16_t i, j;
  strip_C7.setBrightness(BRIGHTNESS_C7);

  for (j = 20; j < 200; j++) {
    for (i = 0; i <strip_C7.numPixels(); i++) {
      strip_C7.setPixelColor(i, j, j/2, 0);
    }
    strip_C7.show();
    delay(1);
  }
  //delay(1500)
}

// strip_C7 pulsing darker
void darken() {
  uint16_t i, j;
  strip_C7.setBrightness(BRIGHTNESS_C7);

  for (j = 200; j > 20; j--) {
    for(i = 0; i < strip_C7.numPixels(); i++) {
      strip_C7.setPixelColor(i, j, j/2, 0);
    }
    strip_C7.show();
    delay(10);
  }
  delay(1);
}

void loop() {

  buttonState_T2 = digitalRead(buttonPin_T2);
  buttonState_T3 = digitalRead(buttonPin_T3);
  buttonState_T4 = digitalRead(buttonPin_T4);

  if (buttonState_T3 == HIGH) {
    if (buttonState_T4 == HIGH) {
      // Fill along the length of the strip in various colors...
      colorWipe(strip_C6.Color(255, 0, 0), 50); // Red
      colorWipe(strip_C6.Color(0, 255, 0), 50); // Green
      colorWipe(strip_C6.Color(0, 0, 255), 50); // Blue
      colorWipe(strip_C6.Color(0, 0, 0, 255), 50); // True white (not RGB white)
    } else {
      // Fill along the length of the strip in various colors...
      colorWipe(strip_C6.Color(100, 0, 100), 50); // Magenta
      colorWipe(strip_C6.Color(0, 50, 50), 50); // Cyan
      //colorWipe(strip_C6.Color(255, 0, 0), 50); // Blue - commented out
      //colorWipe(strip_C6.Color(0, 0, 0, 255), 50); // True white (not RGB white) - commented out
    }
  } else {
    if (buttonState_T4 == HIGH) {
      colorWipe(strip_C6.Color(0, 0, 0), 180);
      colorWipe(strip_C7.Color(0, 100, 0), 60);
    } else {
      colorWipe(strip_C6.Color(180, 0, 0), 0);
      brighten();
      darken();
    }
  }
}

  //whiteOverRainbow(75, 5);

  // pulseWhite(0);

  //rainbowFade2White(3, 3, 1);


void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip_C6.numPixels()) whiteLength = strip_C6.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip_C6.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip_C6.setPixelColor(i, strip_C6.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip_C6.numPixels());
        strip_C6.setPixelColor(i, strip_C6.gamma32(strip_C6.ColorHSV(pixelHue)));
      }
    }

    strip_C6.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip_C6.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip_C6.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip_C7.fill(strip_C7.Color(0, 0, 0, strip_C7.gamma8(j)));
    strip_C7.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip_C7.fill(strip_C7.Color(0, 0, 0, strip_C7.gamma8(j)));
    strip_C7.show();
    delay(wait);
  }
}
/*
void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500); // Pause 1/2 second
}
*/
