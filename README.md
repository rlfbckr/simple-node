# simple-node

boilerplate code for a simple (node based) project running on an ESP 32 + UDP/OSC data exchange + simple update over the air functionality.
e.g.: you want to "control" multiple esp32 from one central computer running processing.

## Reqirments:

### Arduino

- board: ESP32 from Expressif System
- libraries:
  - OSC [https://github.com/CNMAT/OSC](https://github.com/CNMAT/OSC)
  - HTTPupdate (comes with the ESP32 board libraries)
  - HTTPClient (comes with the ESP32 board libraries)
  - AsynUDP (comes with the ESP32 board libraries)
  - Chrono

#### Processing

- libraries:
  - oscP5

#### Node.js (firmware server)

- nodejs + express + ecstatic
  - `npm install express`
  - `npm install ecstatic`

## Over the Air Update (OTA Update)

### Preparing the new firmware
First test the new firmware on one node (use the normal update procedure via USB). if you firmware works you can do a bulk update over the air for multible nodes at the same time.
Make sure to give it a proper incremental version number.

eg: `FW_VERSION=0.50`

the version number will be used to compare the current installed version and the version avaibalbe on the server.

export the binary of your new firmware from arduino.
copy it to the firmware folder `/release/firmware.bin`
change `/release/version.txt` to the new version number.

### initate the update

You have to define ip/address (URL) from where the ESP32 will download the new firmware version and it's binary.

Either you define it directly in the arduino code by setting:

```
const char DEFAULT_URL_FW_VERSION[] = "http://192.168.178.61:8080/release/version.txt";
const char  DEFAULT_URL_FW_BINARY[] = "http://192.168.178.61:8080/release/firmware.bin";`
```

or you can change the location via two OSC commands:

send "`/ufversionurl http://123.1.2.3/release/version.txt`" via OSC from Processing.

send `/ufbinaryurl http://192.168.178.61:8080/release/firmware.bin` via OSC from Processing.

To start the update send `/updatefirmware` via OSC to the ESP32
The ESP32 will request `version.txt` from the URL. if the version on the server is larger than the previous it will download `firmware.bin` and initiate the update.

