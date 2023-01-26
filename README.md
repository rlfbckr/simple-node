# simple-node

Boilerplate code for a simple (node based) project running on an ESP 32 + UDP/OSC data exchange + simple update over the air functionality + node id stored in the EEPROM.
e.g.: you want to "control" multiple esp32 from one central computer running processing.

## Overview

- `simple_node_client_arduino` - ESP32 Node Client
- `firmware updating code` - Processing Server
- `setNodeID_arduino` - writes the NODE_ID to the EEPROM (has to be done once for each node)
- `firmware_server` - Node.js sever to distribute the new firmware binary to all nodes in the network

## Reqirements

### Arduino

- Board: ESP32 from Expressif System
- Libraries:
  - `OSC` ([https://github.com/CNMAT/OSC](https://github.com/CNMAT/OSC))
  - `HTTPupdate` (comes with the ESP32 board libraries)
  - `HTTPClient` (comes with the ESP32 board libraries)
  - `AsyncUDP` (comes with the ESP32 board libraries)
  - `Chrono`

### Processing

- Libraries:
  - `oscP5`

### Node.js (firmware server)

- nodejs + express + ecstatic
  - `npm install express`
  - `npm install ecstatic`

## OSC Messages

- Documentation for the Arduino library: [https://github.com/CNMAT/OSC](https://github.com/CNMAT/OSC)
- Documentation for the Processing library: [https://sojamo.de/libraries/oscP5/](https://sojamo.de/libraries/oscP5/)

OSC messages are usaly composed of an `address pattern` and data eg. integet, floats or string. eg:

`/commmand/parameter 12 1.4 test`

## OSC Unicast vs Broadcast (Processing)

You can send OSC packets directly to one specfic "node" or broadcast it to all "nodes"


### unicast
send directly to the ip of the node
e.g.: `192.1.3.123`

code in processing:
```
  NetAddress myRemoteLocation= new NetAddress("192.1.3.123", 8000);
  oscP5.send(myMessage, myRemoteLocation);
```


### broadcast
send to the broadcast address of the network.

e.g.: `255.255.255.255`

code in processing:
```
  NetAddress myRemoteLocation= new NetAddress("255.255.255.255", 8000);
  oscP5.send(myMessage, myRemoteLocation);
```

## Writing the NODE_ID to the EEPROM

Before you upload the `simple_node_client_arduino` to your ESP32. use the `setNodeID_arduino` program to write its ID to the EEPROM. Change the `set_nodeid` to the desired ID:
```
// set node id here
int set_nodeid = 12; // <------------------ change here to set
```
Afterwards upload the `simple_node_client_arduino` sketch to your ESP32.


## Over the Air Update (OTA Update)

### Preparing the new firmware

First test the new firmware on one of the nodes (use the normal update procedure via USB). Make sure it works to avoid flashing a buggy firmeware to all of your nodes (you might brick them for instance if the update function becomes corrupted). If you firmware works you can do a bulk update over the air for multible nodes at the same time.
Make sure to give it a proper incremental version number.

for example: `FW_VERSION=0.50`

The version number will be used to compare the current installed version (on the ESP32) and the version available on the server.

1. export the binary of your new firmware from arduino. `Sketch > Export compiled binary`.
2. copy it to the firmware folder `/release/firmware.bin`
3. change `/release/version.txt` to the new version number.

### Initate the update

You have to define ip/address (URL) from where the ESP32 will download the new firmware version and it's binary.

Either you define it directly in the arduino code by setting:

```
const char DEFAULT_URL_FW_VERSION[] = "http://192.168.178.61:8080/release/version.txt";
const char  DEFAULT_URL_FW_BINARY[] = "http://192.168.178.61:8080/release/firmware.bin";`
```

Or you can change the location via two OSC commands:

send "`/ufversionurl http://123.1.2.3/release/version.txt`" via OSC from Processing.

send `/ufbinaryurl http://192.168.178.61:8080/release/firmware.bin` via OSC from Processing.

To start the update send `/updatefirmware` via OSC to the ESP32
The ESP32 will request `version.txt` from the URL. if the version on the server is larger than the previous it will download `firmware.bin` and initiate the update.

