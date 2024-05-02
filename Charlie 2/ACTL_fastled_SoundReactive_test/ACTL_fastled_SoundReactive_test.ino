/* First attempt to work with the FastLED library.

 2 Apr 2024
 Kevin Zembower
 A Computer That Loves

 This sketch uses the arrangement of the NeoPixels in the Charlie *2.0*
 mouthpiece.

*/

#include <FastLED.h>
/* #define NUM_LEDS 28 */
#define DATA_PIN 6
#define MIC_PIN   A0                                          // Nano ADC

/* NeoPixels settings */
uint8_t max_bright = 255;

/* Sound processing global variables */
float micLev;

/* The array M maps the physical order of the NeoPixels to the desired
	 order of lighting, from center outwards. For instance, first pixel
	 to light is M[0] = 5, second is M[1] = 14, etc. */
/* Charlie 1.0 mouthpiece array */
/* byte M[] = { */
/*    5, 14,  4, 24,  6, 13, 15, 25, 23,  7,    /\* 0-10 *\/ */
/* 	 3, 12, 26, 22, 16,  8,  2, 21, 17, 11,    /\* 11-20 *\/ */
/* 	27,  9,  1, 10,  2,  8, 20, 18, 19				 /\* 21-28 *\/ */
/* }; */

/* Charlie 2.0 mouthpiece array */
byte M[] = {
	41, 23, 22, 21, 20, 19, 40,
	38, 24,  8,  7,  6, 18, 36,
	33, 25,  9,  1,  5, 17, 32,
	34, 26, 10,  2,  4, 16, 31,
	39, 27, 11, 12,  3, 15, 35,
	41, 28, 29, 30, 13, 14, 39
};

const int num_leds = sizeof(M) / sizeof(M[0]);
CRGB leds[num_leds];

void setup() {
	Serial.begin(115200);
	analogReference(AR_EXTERNAL);     // Nano or other 5V AVR8 when using a 3.3V microphone.
	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, num_leds);

  /* FastLED.setBrightness(max_bright); */
	/* FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, num_leds); */
	delay(1000);
}

void loop() {
	uint16_t sample = getSample();
	uint16_t bars = sample * num_leds / 512; /* convert sample (from 0 to
																						 512) into the number of
																						 LEDs to light */
	/* Serial.print("Bars: "); */
	/* Serial.println(bars); */
	
	for (int i = 0; i < num_leds; i++) {
		leds[M[i]] = (i < bars) ? CRGB::Blue : CRGB::Black;
	}
	FastLED.show();
	delay(100);
}

uint16_t getSample() {
	uint8_t squelch = 100;                         // Value of 100 works best with this mic
  int16_t micIn;
  static uint16_t sample;
  
  micIn = analogRead(MIC_PIN);                  // Sample the microphone. Range will result in 0 to 1023.
  micLev = ((micLev * 31) + micIn) / 32;        // Smooth it out over the last 32 samples.
  
  micIn = micIn - micLev;
  micIn = abs(micIn);

  sample = (micIn <= squelch) ? 0 : (sample + micIn) / 2;   // Using a ternary operator to smooth
	                                                          // out the last 2 (or 4 if you want) samples.
	                                                          // Less glitchy this way.
  Serial.print(600); Serial.print(" ");
  Serial.print(micIn); Serial.print(" ");
  Serial.print(sample); Serial.print(" ");
  Serial.println(" ");

	return sample;
}
