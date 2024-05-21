#include "config.h"
// If this is your first time working with this code,
// you will need to remove the .tpl from the end of the filename

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#ifdef ESP32
// Cannot use 6 as output for ESP. Pins 6-11 are connected to SPI flash. Use 16 instead.
#define LED_PIN    16
#else
#define LED_PIN    6
#endif

// How many NeoPixels are attached to the Arduino?
#define YOUNG_FRUND_LED_COUNT 2

// How bright should the NeoPixels be? (0-255)
#define YOUNG_FRUND_LED_BRIGHTNESS 255 

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(YOUNG_FRUND_LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

uint32_t magenta = strip.Color(255, 0, 255);
uint32_t red = strip.Color(127, 0, 0);
uint32_t blue = strip.Color(0, 104, 255);
uint32_t timeBlue = strip.Color(53, 105, 219);
uint32_t frundGreen = strip.Color(10, 242, 41);

/*
unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
bool          patternComplete = false;

int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels
*/

// WiFi setup - this is _meant_ to be able target multiple board types, YMMV
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif __SAMD21G18A__
#include <WiFi101.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// WIFI_SSID and WIFI_PASS
// (go check config.h)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

IPAddress local_IP(10, 0, 56, 127);
IPAddress gateway(10, 0, 56, 1);
IPAddress subnet(255, 255, 254, 0);

IPAddress primaryDNS(10, 0, 48, 10);   //optional
IPAddress secondaryDNS(10, 0, 48, 11); //optional

int status = WL_IDLE_STATUS;
// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
// const IPAddress outIp(10,40,10,105);        // remote IP (not needed for receive)
// const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 8888;        // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*
unsigned int youngFrundBlueState = 0;
unsigned int youngFrundGreenState = 0;
unsigned int youngFrundSmokeState = 0;

// -----------------------------------------------------------------------

#ifndef BUILTIN_LED
#ifdef LED_BUILTIN
#define BUILTIN_LED LED_BUILTIN
#else
#define BUILTIN_LED 13
#endif
#endif

#define LED_PIN 14 // Currently referring to GPIO 14, which is D5 on the ESP8266 board. Change as needed.

void setup() {

  #ifdef __SAMD21G18A__
    //Configure pins for Adafruit ATWINC1500 Feather
    WiFi.setPins(8,7,4,2);
  #endif

  // The argument  here refers to the baud rate, which can vary depending on what board you're using.
  #ifdef __SAMD21G18A__
    // Baud rate for M0 boards?
    Serial.begin(9600);
  #else
    // Baud rate for most other boards we use???
    Serial.begin(115200);
  #endif
  /* THIS IS A DUMB THING TO INCLUDE, WHY WOULD I DO THIS?
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } */

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  printMacAddress();

  Serial.println("Starting up...");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  /*
  #ifndef __SAMD21G18A__
    // Configures static IP address (the M0s do not seem to need this and it will in fact not compile if you include this)
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
    }
  #endif */
  //WiFi.config(local_IP, primaryDNS, gateway, subnet);
  /*
  #ifdef __SAMD21G18A__
    // Configures static IP address (the M0s do not seem to need this and it will in fact not compile if you include this)
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
    }
  #endif */

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  #ifdef ESP8266
  Serial.println(Udp.localPort());
  #else
  Serial.println(localPort);
  #endif

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(YOUNG_FRUND_LED_BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)

}

// This function will be called by handleOSCMessage()
void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  digitalWrite(LED_PIN, ledState);
  //digitalWrite(BUILTIN_LED, ledState); // This is here if you just want to control the on-board LED
  
  /* Conditional state of OSC message control
  if (ledState == 1) {
    digitalWrite(LED_PIN, ledState);
  } else {
    digitalWrite(LED_PIN, LOW);
  } */

  Serial.print("/led: ");
  Serial.println(ledState);
}

// This function will be called by handleOSCMessage()
void youngFrundBlueModule(OSCMessage &msg) {
  youngFrundBlueState = msg.getInt(0);

  Serial.print("/youngfrundblue: ");
  Serial.println(youngFrundBlueState);
}

// This function will be called by handleOSCMessage()
void youngFrundGreenModule(OSCMessage &msg) {
  youngFrundGreenState = msg.getInt(0);

  Serial.print("/youngfrundgreen: ");
  Serial.println(youngFrundGreenState);
}

// This function will be called by handleOSCMessage()
void youngFrundSmoke(OSCMessage &msg) {
  youngFrundSmokeState = msg.getInt(0);

  Serial.print("/youngfrundsmoke: ");
  Serial.println(youngFrundSmokeState);
}

void handleOSCMessage() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      // This is where you'd change the OSC endpoint name - /led is just the default here
      // Note to self: we should probably standardize our OSC endpoint naming...
      msg.dispatch("/led", led);
      // The second argument, led, is the name of the function you want to call when a message is received.
      // Currently it will call led(), which you can see up above.
      // Make sure this argument matches the name of whatever function you write to handle behavior!

      msg.dispatch("/youngfrundblue", youngFrundBlueModule);
      msg.dispatch("/youngfrundgreen", youngFrundGreenModule);
      msg.dispatch("youngfrundsmoke", youngFrundSmoke);

    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

void loop() {
  handleOSCMessage(); // Just check for incoming OSC messages, forever

  switch (youngFrundBlueState) {
  case 0:
    // statements
    // Turn off the blue module!
    strip.setPixelColor(0, 0, 0, 0);
    strip.show();
    break;
  case 1:
    // statements
    // Turn on the blue module!
    strip.setPixelColor(0, blue);
    strip.show();
    break;
  default:
    // OFF (failstate?)
    strip.clear();
    strip.show();
    break;
  }

  switch (youngFrundGreenState) {
  case 0:
    // OFF
    // Turn off the green module!
    strip.setPixelColor(1, 0, 0, 0);
    strip.show();
    break;
  case 1:
    // Turn on the green module!
    strip.setPixelColor(1, frundGreen);
    strip.show();
    break;
  default:
    // OFF (failstate?)
    strip.clear();
    strip.show();
    break;
  }

}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printMacAddress() {
  byte mac[6];
  WiFi.macAddress(mac);
  
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}
