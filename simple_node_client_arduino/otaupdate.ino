
int getFirmwareVersionFromServer() {
  String payload;
  int httpCode;
  String fwurl = "";
  fwurl += URL_FW_VERSION;
  fwurl += "?";
  fwurl += String(rand());
  Serial.println(fwurl);
  WiFiClient * client = new WiFiClient;
  LOCK_UDP_REICEIVER = true; // lock the udp/osc process to avoid accidents ;)
  delay(200);
  if (client) {
    HTTPClient http;
    if (http.begin( * client, fwurl))  { // HTTPS
      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      delay(100);
      httpCode = http.GET();
      delay(100);
      if (httpCode == HTTP_CODE_OK) {
        payload = http.getString(); // save received version
      } else {
        Serial.print("error in downloading version file:");
      }
      http.end();
    }
    delete client;
    LOCK_UDP_REICEIVER = false;
  }


  if (httpCode == HTTP_CODE_OK) { // if version received
    payload.trim();
    float remote_version = payload.toFloat();
    Serial.print("FW UPDATE> remote_version = ");
    Serial.println(remote_version);
    Serial.print("FW UPDATE> FW_VEERSON (local) = ");
    Serial.println(FW_VERSION);
    LOCK_UDP_REICEIVER = false;
    if (remote_version > FW_VERSION) {
      Serial.println("FW UPDATE> New firmware detected");

      LOCK_UDP_REICEIVER = false;
      return 1;
    } else {
      Serial.println("FW UPDATE> Device already on latest firmware version.");
      LOCK_UDP_REICEIVER = false;
      return 0;
    }
  }
  LOCK_UDP_REICEIVER = false;
  return 0;
}

void updateFirmwareFromServer() {
  WiFiClient client;
  Serial.println("FW UPDATE> starting update...");
  Serial.print("FW UPDATE> BIN_URL = ");
  Serial.println(URL_FW_BINARY);
  LOCK_UDP_REICEIVER = true;
#ifdef ESP8266
  t_httpUpdate_return ret = ESPhttpUpdate.update(client, URL_FW_BINARY);
#else
  t_httpUpdate_return ret = httpUpdate.update(client, URL_FW_BINARY);
#endif
  Serial.println("FW UPDATE> DONE");
  Serial.print("FW UPDATE> ret = ");
  Serial.println(ret);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
#ifdef ESP8266
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
#else
      Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
#endif
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
  LOCK_UDP_REICEIVER = false;
}

void update_started() {
  Serial.println("CALLBACK:  HTTP update process started");
}

void update_finished() {
  Serial.println("CALLBACK:  HTTP update process finished");
}

void update_progress(int cur, int total) {
  Serial.printf("CALLBACK:  HTTP update process at %d of %d bytes...\n", cur, total);
}

void update_error(int err) {
  Serial.printf("CALLBACK:  HTTP update fatal error code %d\n", err);
}
