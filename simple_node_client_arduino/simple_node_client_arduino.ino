#ifdef ESP8266
// take specific libraries for ESP8266
// board file from here: https://arduino.esp8266.com/stable/package_esp8266com_index.json
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ESPAsyncUDP.h> // https://github.com/me-no-dev/ESPAsyncUDP
#else
// libs for ESP32
// board file from here: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "AsyncUDP.h"
#endif

#include <Chrono.h>
#include <OSCBundle.h>
#include <EEPROM.h> // 

#define EEPROM_SIZE 12


int MY_NODE_ID = -1; // the final MY_NODE_ID is not set here, it will be stored and read from EEPROM.
                     // this will allow you to use the "same" code on
                     // all nodes without setting the node in the code here.
                     // before you have to set (write to the eeprom) the node ID via the setNodeID arduino sketch.
                     // upload this sketch afterwads. 
                     
float FW_VERSION = 0.01; // important for the firmware ota flashing process

// server location of your new firmware (export firmware with arduino IDE , change version.txt as well)
// change server IP if needed
// can be set via osc as well

const char DEFAULT_URL_FW_VERSION[] = "http://192.168.178.61:8080/release/version.txt";
const char  DEFAULT_URL_FW_BINARY[] = "http://192.168.178.61:8080/release/firmware.bin";

boolean LOCK_UDP_REICEIVER = true; // lock UDP/OSC receiver to avoid shit while flashing a new firmware
char URL_FW_VERSION[512];
char URL_FW_BINARY[512];
boolean UPDATE_FIRMWARE = false; // hook in firmwareupdate

long pingInterval = 2000; // every 2 seconds
int networkLocalPort = 8888;
int networkOutPort = 9999; // remote port to receive OSC

#ifdef ESP8266
ESP8266WiFiMulti wifiMulti;
#else
WiFiMulti wifiMulti;
#endif
Chrono pingTimer;
AsyncUDP udp;
AsyncUDP udpOut;

void setup() {
  strncpy(URL_FW_VERSION, DEFAULT_URL_FW_VERSION, strlen(DEFAULT_URL_FW_VERSION));
  strncpy(URL_FW_BINARY, DEFAULT_URL_FW_BINARY, strlen(DEFAULT_URL_FW_BINARY));
  MY_NODE_ID = readNodeIDfromEEPROM();
  Serial.begin(115200);
  Serial.print("--> NODE  v:");
  Serial.println(FW_VERSION);
  initWIFI();
  initUDP();
}

void loop() {
  if (UPDATE_FIRMWARE) {
    if (getFirmwareVersionFromServer()) { // check if a new version is avaiable on the server
      updateFirmwareFromServer(); // get binary and flash it.
    }
    UPDATE_FIRMWARE = false; // update done
  }
  if (pingTimer.hasPassed(pingInterval)) {
    pingTimer.restart();
    sendPingOSC();
  }
}
