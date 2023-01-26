# simple_node

boilerplate for a simple node based project running on an ESP 32 + UDP/OSC data exchange + simple update over the air functionality

## reqirments

### Arduino

- board: ESP32 from Expressif System
- libraries:
  - OSC
  - ESP32 HTTPupdate
  - HTTPClient
  - Chrono

#### Processing

- libraries:
  - oscP5

## Node.js (Firmware Server)

- nodejs + express + ecstatic
  - `npm install express`
  - `npm install ecstatic`