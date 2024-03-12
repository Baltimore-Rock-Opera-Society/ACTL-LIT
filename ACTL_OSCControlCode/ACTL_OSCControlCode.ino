#include "config.h"
// If this is your first time working with this code,
// you will need to remove the .tpl from the end of the filename

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

// WIFI_SSID and WIFI_PASS
// (go check config.h)

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(10,40,10,105);        // remote IP (not needed for receive)
const unsigned int outPort = 9999;          // remote port (not needed for receive)
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

  // The argument  here refers to the baud rate, which can vary depending on what board you're using.
  Serial.begin(9600);
  Serial.println("Starting up...");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);

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
  #ifdef ESP32
  Serial.println(localPort);
  #else
  Serial.println(Udp.localPort());
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
