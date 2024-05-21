#include "config.h"
// If this is your first time working with this code,
// you will need to remove the .tpl from the end of the filename

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

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

IPAddress local_IP(10, 0, 56, 214);
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
unsigned int helmetState;

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
  /* THIS IS A REALLY DUMB THING TO INCLUDE
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

  /*
  #ifndef __SAMD21G18A__
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

  // Maybe flash the neopixels to indicate connection to network???
  NeoPixel_MOHAWK_L.setColor("WHITE");
  NeoPixel_MOHAWK_R.setColor("WHITE");

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

  handleOSCMessage(); // Just check for incoming OSC messages, forever

  switch (helmetState) {
  case 0:
    // OFF
    //Serial.println("This should turn the helmet off!");
    NeoPixel_SIDE_L.clearPixels();
    NeoPixel_SIDE_R.clearPixels();
    NeoPixel_BLOB_L.clearPixels();  
    NeoPixel_BLOB_R.clearPixels();
    NeoPixel_MOHAWK_L.clearPixels();
    NeoPixel_MOHAWK_R.clearPixels();
    //strip.clear();
    //strip.show();
    break;
  case 1:
    // HAPPINESS - YELLOW
    NeoPixel_SIDE_L.setCustomColor(246, 235, 0);
    NeoPixel_SIDE_R.setCustomColor(246, 235, 0);
    NeoPixel_BLOB_L.setCustomColor(246, 235, 0);
    NeoPixel_MOHAWK_L.setCustomColor(246, 235, 0);
    NeoPixel_BLOB_R.setCustomColor(246, 235, 0);
    NeoPixel_MOHAWK_R.setCustomColor(246, 235, 0);
    //strip.show();
    break;
  case 2:
    // SADNESS - PURPLE
    // Turn all pixels red? (test for now)
    // 184, 51, 255
    NeoPixel_SIDE_L.setCustomColor(77, 3, 156);
    NeoPixel_SIDE_R.setCustomColor(77, 3, 156);
    NeoPixel_BLOB_L.setCustomColor(77, 3, 156);
    NeoPixel_MOHAWK_L.setCustomColor(77, 3, 156);
    NeoPixel_BLOB_R.setCustomColor(77, 3, 156);
    NeoPixel_MOHAWK_R.setCustomColor(77, 3, 156);
    //strip.show();
    break;
  case 3:
    // LOVE - PINK (but fails to get through, so maybe a "sputtering" animation effect here?)
    // Turn all pixels pink? (test for now)
    // TO-DO: Maybe add quick flashes to purple, then a "sputter"?
    //Serial.println("This should change the helmet to red!");
    //strip.fill(red, 0, ITEM_LED_COUNT);
    NeoPixel_SIDE_L.setCustomColor(255, 25, 175);
    NeoPixel_SIDE_R.setCustomColor(255, 25, 175);
    NeoPixel_BLOB_L.setCustomColor(255, 25, 175);
    NeoPixel_MOHAWK_L.setCustomColor(255, 25, 175);
    NeoPixel_BLOB_R.setCustomColor(255, 25, 175);
    NeoPixel_MOHAWK_R.setCustomColor(255, 25, 175);
    delay(random(150,300));
    NeoPixel_SIDE_L.setCustomColor(169, 24, 222);
    NeoPixel_SIDE_R.setCustomColor(169, 24, 222);
    NeoPixel_BLOB_L.setCustomColor(169, 24, 222);
    NeoPixel_MOHAWK_L.setCustomColor(169, 24, 222);
    NeoPixel_BLOB_R.setCustomColor(169, 24, 222);
    NeoPixel_MOHAWK_R.setCustomColor(169, 24, 222);
    delay(random(350,550));
    // 8, 70, 204
    NeoPixel_SIDE_L.setCustomColor(8, 70, 204);
    NeoPixel_SIDE_R.setCustomColor(8, 70, 204);
    NeoPixel_BLOB_L.setCustomColor(8, 70, 204);
    NeoPixel_MOHAWK_L.setCustomColor(8, 70, 204);
    NeoPixel_BLOB_R.setCustomColor(8, 70, 204);
    NeoPixel_MOHAWK_R.setCustomColor(8, 70, 204);
    //strip.show();
    break;
  case 4:
    // PANIC - WHITE FLASH (???)
    NeoPixel_SIDE_L.setColor("WHITE");
    NeoPixel_SIDE_R.setColor("WHITE");
    NeoPixel_BLOB_L.setColor("WHITE");
    NeoPixel_MOHAWK_L.setColor("WHITE");
    NeoPixel_BLOB_R.setColor("WHITE");
    NeoPixel_MOHAWK_R.setColor("WHITE");
    // delay() is bad and I should feel bad.
    // I could add another OSC endpoint for controlling transition time,
    // and send a fade from QLab to dynamically control how fast/slow the pulses are
    // (to allow for more a controlled hyperventilating effect, where Charlie's
    // "breathing" speeds up and then reasserts itself as he calms down, etc)
    delay(random(50,100));
    NeoPixel_SIDE_L.clearPixels();
    NeoPixel_SIDE_R.clearPixels();
    NeoPixel_BLOB_L.clearPixels();  
    NeoPixel_BLOB_R.clearPixels();
    NeoPixel_MOHAWK_L.clearPixels();
    NeoPixel_MOHAWK_R.clearPixels();
    delay(random(100,200));
    //strip.show();
    break;
  default:
    // OFF (failstate?)
    NeoPixel_SIDE_L.clearPixels();
    NeoPixel_SIDE_R.clearPixels();
    NeoPixel_BLOB_L.clearPixels();  
    NeoPixel_BLOB_R.clearPixels();
    NeoPixel_MOHAWK_L.clearPixels();
    NeoPixel_MOHAWK_R.clearPixels();
    //strip.clear();
    //strip.show();
    break;
  }

  /*
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
  */

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


// This function will be called by handleOSCMessage()
void charlieHelmet(OSCMessage &msg) {
  helmetState = msg.getInt(0);
  //digitalWrite(LED_PIN, helmetState);
  //digitalWrite(BUILTIN_LED, ledState); // This is here if you just want to control the on-board LED

  /* Conditional state of OSC message control
  if (ledState == 1) {
    digitalWrite(LED_PIN, ledState);
  } else {
    digitalWrite(LED_PIN, LOW);
  } */

  Serial.print("/charliehelmet: ");
  Serial.println(helmetState);
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
      //msg.dispatch("/led", led);
      msg.dispatch("/charliehelmet", charlieHelmet); // Change "charliehelmet" and "charlieHelmet" to whatever you're calling
      // your current work project!
      // The second argument, led, is the name of the function you want to call when a message is received.
      // Currently it will call led(), which you can see up above.
      // Make sure this argument matches the name of whatever function you write to handle behavior!
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  //IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  //Serial.println(ip);
  Serial.println(WiFi.localIP());

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

