# simple_node

boilerplate code for a simple (node based) project running on an ESP 32 + UDP/OSC data exchange + simple update over the air functionality.
e.g.: you want to "control" multiple esp32 from one central computer running processing.

## reqirments

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

## Node.js (Firmware Server)

- nodejs + express + ecstatic
  - `npm install express`
  - `npm install ecstatic`