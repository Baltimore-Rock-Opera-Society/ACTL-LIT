#include "config.h"
// If this is your first time working with this code,
// you will need to remove the .tpl from the end of the filename

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
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

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

  #ifndef __SAMD21G18A__
    // Configures static IP address (the M0s do not seem to need this and it will in fact not compile if you include this)
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
      Serial.println("STA Failed to configure");
    }
  #endif

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
    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

void loop() {
  handleOSCMessage(); // Just check for incoming OSC messages, forever
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
