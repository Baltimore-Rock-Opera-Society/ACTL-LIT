//
#include "PixelStrip.h"
#include <vector>

PixelStrip::PixelStrip(String whichStrip, uint8_t pin, neoPixelType type) : pixels(setStripType(whichStrip), pin, type) {
    pixels.begin();
    pixels.setBrightness(setStripBrightness(whichStrip));
    // rainbow_colors.resize(pixels.numPixels());
    // for (uint16_t i = 0; i < pixels.numPixels(); i++){
    //   rainbow_colors[i] = 0;
    // }
    rainbow_colors.resize(setStripType(whichStrip));
    // uint8_t delta =  255 / (setStripType(whichStrip));
    // for (uint16_t i = 0; i < setStripType(whichStrip); i++) {
    //   rainbow_colors[i] = delta *i; //fill the entire array with the 
    // }
    // for (uint16_t i = 0; i < setStripType(whichStrip); i++){
    //   rainbow_colors[i] = 0;
    // }

    uint8_t delta =  255 / (pixels.numPixels());
    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
      rainbow_colors[i] = delta *i; //fill the entire array with the 
    }
    
}

void PixelStrip::clearPixels() {
  pixels.clear();
  pixels.show();
}

uint16_t PixelStrip::setStripType(String whichStrip) {
    numPixels = 1;
    if (whichStrip == "MOHAWK"){
      numPixels = 27;
		} else if (whichStrip  == "SIDE"){
			numPixels = 20;
		} else if (whichStrip  == "BLOB"){
			numPixels = 17;
		} else {
			Serial.println("Invalid Strip Argument");
			return 0;
		}
    return numPixels;
}

uint16_t PixelStrip::setStripBrightness(String whichStrip) {
    numPixels = 1;
    if (whichStrip == "MOHAWK"){
      numPixels = 50;
		} else if (whichStrip  == "SIDE"){
			numPixels = 100;
		} else if (whichStrip  == "BLOB"){
			numPixels = 50;
		} else {
			Serial.println("Invalid Strip Argument");
			return 0;
		}
    return numPixels;
}


void PixelStrip::setCustomColor(uint8_t red, uint8_t green, uint8_t blue) {
    color = pixels.Color(red, green, blue);

    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, color);
    }
    pixels.show();

}

void PixelStrip::setCustomColorWithDelay(uint8_t red, uint8_t green, uint8_t blue, uint32_t delayTime) {
    color = pixels.Color(red, green, blue);

    for (uint16_t i = 0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, color);
        pixels.show();
        delay(delayTime); // Custom delay after setting the color
    }
}

void PixelStrip::setColor(String input_color) {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

		if (input_color == "RED"){
			red = 255;
      green = 0;
      blue = 0;
		} else if (input_color == "GREEN"){
			red = 0;
      green = 255;
      blue = 0;
		} else if (input_color == "BLUE"){
			red = 0;
      green = 0;
      blue = 255;
		} else if (input_color == "WHITE"){
			red = 255;
      green = 255;
      blue = 255;
		} else {
			Serial.println("Invalid Color Argument");
			return;
		}
		
		for (uint16_t i = 0; i < pixels.numPixels(); i++) {
			pixels.setPixelColor(i, red, green, blue);
		}
		pixels.show();

}

void PixelStrip::setColorWithDelay(String input_color, uint32_t DELAY_INTERVAL) {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

		if (input_color == "RED"){
			red = 255;
      green = 0;
      blue = 0;
		} else if (input_color == "GREEN"){
			red = 0;
      green = 255;
      blue = 0;
		} else if (input_color == "BLUE"){
			red = 0;
      green = 0;
      blue = 255;
		} else if (input_color == "WHITE"){
			red = 255;
      green = 255;
      blue = 255;
		} else {
			Serial.println("Invalid Color Argument");
			return;
		}
		
		for (uint16_t i = 0; i < pixels.numPixels(); i++) {
			pixels.setPixelColor(i, red, green, blue);
			pixels.show();
			delay(DELAY_INTERVAL);
		}

}
void PixelStrip::rainbow() {
  uint8_t delta = 255 / pixels.numPixels(); // Calculate the color change step
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, Wheel(delta * i)); // Set each pixel to a rainbow color
  }
  pixels.show();
}

void PixelStrip::rainbowDelay(uint32_t DELAY_INTERVAL) {
  uint8_t delta = 255 / pixels.numPixels(); // Calculate the color change step
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    // how do I make the color move down as a chunk?

    pixels.setPixelColor(i, Wheel(delta * i)); // Set each pixel to a rainbow color
    pixels.show();
		delay(DELAY_INTERVAL);
  }
}

void PixelStrip::rainbowShiftLoop(uint32_t DELAY_INTERVAL, uint32_t howManyLoops) {

  for (uint16_t k = 0; k < pixels.numPixels()*howManyLoops; k++) {
    uint16_t i = fmod(k, pixels.numPixels());
    uint16_t fill_in = i;
    while(fill_in > 0){
      pixels.setPixelColor(fill_in, Wheel(rainbow_colors[i-fill_in]));
      fill_in --;
    }
    pixels.show();
    delay(DELAY_INTERVAL);
  }
}


void PixelStrip::blinkColor(String input_color, uint32_t DELAY_INTERVAL, uint32_t howManyLoops){
  for (uint16_t k = 0; k < howManyLoops; k++){
    setColor(input_color);
    delay(DELAY_INTERVAL);
  }
  setCustomColor(0, 0, 0);
  delay(DELAY_INTERVAL);
    
}

void PixelStrip::blinkCustomColor(uint8_t red, uint8_t green, uint8_t blue, uint32_t DELAY_INTERVAL, uint32_t howManyLoops){
  for (uint16_t k = 0; k < howManyLoops; k++){
    setCustomColor(red, green, blue);
    delay(DELAY_INTERVAL);
  }
  setCustomColor(0, 0, 0);
  delay(DELAY_INTERVAL);
    
}

void PixelStrip::rainbowBlink(uint32_t DELAY_INTERVAL, uint32_t howManyLoops){
  for (uint16_t i = 0; i < howManyLoops; i++){
    for (uint16_t k = 0; k < pixels.numPixels(); k++){
      Serial.println(Wheel(rainbow_colors[k]));
      uint8_t red = (Wheel(rainbow_colors[k]) >> 16) & 0xFF;
      uint8_t green = (Wheel(rainbow_colors[k]) >> 8) & 0xFF;
      uint8_t blue = (Wheel(rainbow_colors[k])>>0) & 0xFF;
      setCustomColor(red, green, blue);
      delay(DELAY_INTERVAL);
      setCustomColor(0, 0, 0);
      delay(DELAY_INTERVAL);
    }
    // setCustomColor(0, 0, 0);
    // delay(DELAY_INTERVAL);
  }
}

void PixelStrip::rainbowTransition(uint32_t DELAY_INTERVAL, uint32_t howManyLoops){
  for (uint16_t i = 0; i < howManyLoops; i++){
    for (uint16_t k = 0; k < pixels.numPixels(); k++){
      Serial.println(Wheel(rainbow_colors[k]));
      uint8_t red = (Wheel(rainbow_colors[k]) >> 16) & 0xFF;
      uint8_t green = (Wheel(rainbow_colors[k]) >> 8) & 0xFF;
      uint8_t blue = (Wheel(rainbow_colors[k])>>0) & 0xFF;
      setCustomColor(red, green, blue);
      delay(DELAY_INTERVAL);
    }
  }
}


uint32_t PixelStrip::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
      return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else if (WheelPos < 170) {
      WheelPos -= 85;
      return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } else {
      WheelPos -= 170;
      return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
  }





