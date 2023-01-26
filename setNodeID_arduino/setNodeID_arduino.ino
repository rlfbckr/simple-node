#include <EEPROM.h> // 
#define EEPROM_SIZE 12

// writes nodes id to the EEPROM
// (it won't be overwritten by flasing the firmware)




// set node id here
int set_nodeid = 12; // <------------------ change here to set


















// ------------------------------------------------------
// dont change anything below!
int read_nodeid = -1;

void setup() {
  Serial.begin(115200);
  Serial.print("SET nodeid = ");
  Serial.println(set_nodeid);
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.write(0, set_nodeid);
  EEPROM.end();
}

void loop() {
  EEPROM.begin(EEPROM_SIZE);
  read_nodeid =  EEPROM.read(0);
  EEPROM.end();
  Serial.print(millis());
  Serial.print(" READ_nodeid = ");
  Serial.print(read_nodeid);
  Serial.print(" ( ");
  Serial.print(set_nodeid);
  Serial.println(" )");
  delay(1000);
}
