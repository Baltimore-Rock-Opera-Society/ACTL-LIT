/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-neopixel-led-strip
 */

#include <Adafruit_NeoPixel.h>
#include "PixelStrip.h"
#ifdef __AVR__
#include <avr/power.h>  // Required for 16 MHz Adafruit Trinket
#endif

#define PIN_MOHAWK_L 5  // Arduino pin that connects to NeoPixel
#define PIN_SIDE_L 6  // Arduino pin that connects to NeoPixel


// Three different Options: "MOHAWK", "SIDE", "BLOB"
PixelStrip NeoPixel_MOHAWK("MOHAWK", PIN_MOHAWK, NEO_GRB + NEO_KHZ800);
void setup() {
  //NeoPixel_MOHAWK.clearPixels(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
}

void loop() {
  //NeoPixel_MOHAWK.setColor("RED");
  //delay(2000);      // off time
  // turn off all pixels for two seconds

  // Color as String (Options "RED", "GREEN", "BLUE", "WHITE", delay between each pixel )
  //NeoPixel_MOHAWK.setColorWithDelay("GREEN", 200);
  //delay(2000);      // off time

  //delay(2000);      // off time

  NeoPixel_MOHAWK.rainbowTransition(100, 500);

}

//cycle
