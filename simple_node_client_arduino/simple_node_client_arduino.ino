#include <esp_wifi.h>
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include "soc/rtc_cntl_reg.h"
#include "soc/soc.h"
#include <Chrono.h>
#include <OSCBundle.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include "AsyncUDP.h"
int MY_NODE_ID = 12; // can also be stored in eeprom

float FW_VERSION = 0.01;

// server location of your new firmware (export firmware with arduino IDE , change version.txt as well)
// change server IP if needed

const char DEFAULT_URL_FW_VERSION[] = "http://192.168.178.61:8080/release/version.txt"; 
const char  DEFAULT_URL_FW_BINARY[] = "http://192.168.178.61:8080/release/firmware.bin";
boolean UPDATE_FIRMWARE = true; // hook in firmwareupdate

long pingInterval = 2000; // every 2 seconds
int networkLocalPort = 8888;
int networkOutPort = 9999; // remote port to receive OSC

WiFiMulti wifiMulti;
Chrono pingTimer;
AsyncUDP udp;
AsyncUDP udpOut;

void setup() {
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
