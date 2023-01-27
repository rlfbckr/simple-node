/*
  readNodeIDfromEEPROM()
  
  this function reads the NODE_ID (byte) from the EEPROM
  (the eeprom is not overwritten when uploading a new script)
*/

int readNodeIDfromEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  int read_nodeid =  EEPROM.read(0); // reads the first byte from the EEPROM
  EEPROM.end();
  Serial.print("READ_nodeid = ");
  Serial.println(read_nodeid);
  if (read_nodeid >= 0) {
    return read_nodeid;
  }
  return -1;
}
