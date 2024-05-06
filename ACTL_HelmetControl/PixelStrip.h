// PixelStripControl_h
#ifndef PixelStrip_h
#define PixelStrip_h

// better to organize as one strip rather than different units?
// do one strip for now just to keep the color control in one place
// easier to make classes out of big umbrella

#include <Arduino.h>
#include <vector>
#include <Adafruit_NeoPixel.h>

class PixelStrip {
  private:
    String whichStrip;
    std::vector<uint32_t> rainbow_colors;
    uint32_t color;
    uint32_t howManyLoops;
    uint16_t numPixels;
    uint32_t nextColor;
   
    Adafruit_NeoPixel pixels;

  public:
    PixelStrip(String whichStrip, uint8_t pin, neoPixelType type);
    
    uint16_t setStripType(String whichStrip);
    uint32_t Wheel(byte WheelPos);

    void clearPixels();

    void setColor(String input_color);
    void setColorWithDelay(String input_color, uint32_t DELAY_INTERVAL);
    void blinkColor(String input_color, uint32_t DELAY_INTERVAL, uint32_t howManyLoops);
    
    void setCustomColor(uint8_t red, uint8_t green, uint8_t blue);
    void setCustomColorWithDelay(uint8_t red, uint8_t green, uint8_t blue, uint32_t DELAY_INTERVAL);
    void blinkCustomColor(uint8_t red, uint8_t green, uint8_t blue, uint32_t DELAY_INTERVAL, uint32_t howManyLoops);

    void rainbow();
    void rainbowDelay(uint32_t DELAY_INTERVAL);
    void rainbowShiftLoop(uint32_t DELAY_INTERVAL, uint32_t howManyLoops);
    void rainbowBlink(uint32_t DELAY_INTERVAL, uint32_t howManyLoops);
    void rainbowTransition(uint32_t DELAY_INTERVAL, uint32_t howManyLoops);
};
#endif
