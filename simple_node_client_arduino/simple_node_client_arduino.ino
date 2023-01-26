#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <Chrono.h>
#include <OSCBundle.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "AsyncUDP.h"
#include <EEPROM.h> // 

#define EEPROM_SIZE 12


int MY_NODE_ID = -1; // the final MY_NODE_ID is not set here, it will be read from EEPROM.
                     // this will allow you to use the "same" code an
                     // all nodes without setting the node in the code
                     // before you have to set the node ID via the setNodeID arduino sketch.
                     // upload it and afterwads upload the sketch
                     
float FW_VERSION = 0.01; // important for the firmware ota flashing process

// server location of your new firmware (export firmware with arduino IDE , change version.txt as well)
// change server IP if needed
// can be set via osc as well

const char DEFAULT_URL_FW_VERSION[] = "http://192.168.178.61:8080/release/version.txt";
const char  DEFAULT_URL_FW_BINARY[] = "http://192.168.178.61:8080/release/firmware.bin";

boolean LOCK_UDP_REICEIVER = true; // lock UDP/OSC receiver to avoid shit while flashing a new firmware
char URL_FW_VERSION[512];
char URL_FW_BINARY[512];
boolean UPDATE_FIRMWARE = true; // hook in firmwareupdate

long pingInterval = 2000; // every 2 seconds
int networkLocalPort = 8888;
int networkOutPort = 9999; // remote port to receive OSC

WiFiMulti wifiMulti;
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
    if (getFirmwareVersionFromServer()) { // check for no
      updateFirmwareFromServer();
    }
    UPDATE_FIRMWARE = false;
  }
  if (pingTimer.hasPassed(pingInterval)) {
    pingTimer.restart();
    sendPingOSC();
  }
}
