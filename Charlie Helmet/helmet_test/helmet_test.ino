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

#define PIN_BLOB_R 11
#define PIN_SIDE_R 12  // Arduino pin that connects to NeoPixel
#define PIN_MOHAWK_R 13  // Arduino pin that connects to NeoPixel


#define PIN_BLOB_L 5
#define PIN_SIDE_L 6  // Arduino pin that connects to NeoPixel
#define PIN_MOHAWK_L 10  // Arduino pin that connects to NeoPixel


// Three different Options: "MOHAWK", "SIDE", "BLOB"
PixelStrip NeoPixel_MOHAWK_L("MOHAWK", PIN_MOHAWK_L, NEO_GRB + NEO_KHZ800);
PixelStrip NeoPixel_SIDE_L("SIDE", PIN_SIDE_L, NEO_GRB + NEO_KHZ800);
PixelStrip NeoPixel_BLOB_L("BLOB", PIN_BLOB_L, NEO_GRB + NEO_KHZ800);

PixelStrip NeoPixel_MOHAWK_R("MOHAWK", PIN_MOHAWK_R, NEO_GRB + NEO_KHZ800);
PixelStrip NeoPixel_SIDE_R("SIDE", PIN_SIDE_R, NEO_GRB + NEO_KHZ800);
PixelStrip NeoPixel_BLOB_R("BLOB", PIN_BLOB_R, NEO_GRB + NEO_KHZ800);

void setup() {
  //NeoPixel_MOHAWK.clearPixels(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  NeoPixel_SIDE_L.clearPixels();
  NeoPixel_SIDE_R.clearPixels();

  NeoPixel_BLOB_L.clearPixels();  
  NeoPixel_BLOB_R.clearPixels();

  NeoPixel_MOHAWK_L.clearPixels();
  NeoPixel_MOHAWK_R.clearPixels();
}

void loop() {
  // note to self, make a crazy mode where all the strips go nuts

  NeoPixel_SIDE_L.setColor("WHITE");
  NeoPixel_SIDE_R.setColor("WHITE");
  NeoPixel_BLOB_L.setColor("WHITE");
  NeoPixel_MOHAWK_L.setColor("WHITE");
  NeoPixel_BLOB_R.setColor("WHITE");
  NeoPixel_MOHAWK_R.setColor("WHITE");
  delay(500);
  NeoPixel_SIDE_L.setColor("BLUE");
  NeoPixel_SIDE_R.setColor("BLUE");
  NeoPixel_BLOB_L.setColor("BLUE");
  NeoPixel_MOHAWK_L.setColor("BLUE");
  NeoPixel_BLOB_R.setColor("BLUE");
  NeoPixel_MOHAWK_R.setColor("BLUE");
  delay(500);
  NeoPixel_SIDE_L.setColor("GREEN");
  NeoPixel_SIDE_R.setColor("GREEN");
  NeoPixel_BLOB_L.setColor("GREEN");
  NeoPixel_MOHAWK_L.setColor("GREEN");
  NeoPixel_BLOB_R.setColor("GREEN");
  NeoPixel_MOHAWK_R.setColor("GREEN");
  delay(500);
  NeoPixel_SIDE_L.setColor("RED");
  NeoPixel_SIDE_R.setColor("RED");
  NeoPixel_BLOB_L.setColor("RED");
  NeoPixel_MOHAWK_L.setColor("RED");
  NeoPixel_BLOB_R.setColor("RED");
  NeoPixel_MOHAWK_R.setColor("RED");
  delay(500);

  NeoPixel_SIDE_L.clearPixels();
  NeoPixel_SIDE_R.clearPixels();
  NeoPixel_BLOB_L.clearPixels();  
  NeoPixel_BLOB_R.clearPixels();
  NeoPixel_MOHAWK_L.clearPixels();
  NeoPixel_MOHAWK_R.clearPixels();

  // NeoPixel_SIDE_L.setColor("BLUE");
  // NeoPixel_SIDE_R.setColor("BLUE");
  // delay(2000);
  // NeoPixel_SIDE_L.clearPixels();
  // NeoPixel_SIDE_R.clearPixels();
  // delay(2000);
  // NeoPixel_SIDE_L.setColor("WHITE");
  // NeoPixel_SIDE_R.setColor("WHITE");
  // delay(2000); 
  // NeoPixel_SIDE_L.clearPixels();
  // NeoPixel_SIDE_R.clearPixels();


  // NeoPixel_BLOB_L.rainbowTransition(20, 10);
  // NeoPixel_BLOB_R.rainbowTransition(20, 10);
  // delay(2000);
  
  // NeoPixel_BLOB_L.clearPixels();  
  // NeoPixel_BLOB_R.clearPixels();
  // NeoPixel_MOHAWK_L.rainbowShiftLoop(20,10);
  // NeoPixel_MOHAWK_R.rainbowShiftLoop(20,10);
  // delay(2000);
  // delay(100);
  // NeoPixel_MOHAWK_L.rainbowTransition(20,10);
  // NeoPixel_MOHAWK_R.rainbowTransition(20,10);
  // delay(2000);
  // NeoPixel_MOHAWK_L.clearPixels();
  // NeoPixel_MOHAWK_R.clearPixels();
  // delay(5000);
  // NeoPixel_MOHAWK.setColor("RED");
  // delay(2000);      // off time
  // turn off all pixels for two seconds

  // Color as String (Options "RED", "GREEN", "BLUE", "WHITE", delay between each pixel )
  //NeoPixel_MOHAWK.setColorWithDelay("GREEN", 200);
  //delay(2000);      // off time

  //delay(2000);      // off time
  

  // Issue with rainbow transition code - I can't have all three strips at one
  // NeoPixel_MOHAWK_L.setColorWithDelay("WHITE", 200);
  // NeoPixel_SIDE_L.setColorWithDelay("WHITE", 200);
  // NeoPixel_BLOB_L.setColorWithDelay("WHITE", 200);
  // NeoPixel_MOHAWK_L.rainbowTransition(10, 200);
}

//cycle
