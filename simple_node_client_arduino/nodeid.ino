int readNodeIDfromEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  int read_nodeid =  EEPROM.read(0);
  EEPROM.end();
  Serial.print(" READ_nodeid = ");
  Serial.println(read_nodeid);
  if (read_nodeid >= 0) {
    return read_nodeid;
  }
}
