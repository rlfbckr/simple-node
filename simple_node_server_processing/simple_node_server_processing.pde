import oscP5.*;
import netP5.*;

OscP5 oscP5;

String remoteIP = "127.0.0.1";
int remotePort = 8888;
long lastsend = 0;

void setup() {
  size(800, 800);
  oscP5 = new OscP5(this, 9999);
}


void draw() {
  background(0);
  if (millis()-lastsend>1000) {
    // sends message evey second!
    background(255);
    lastsend = millis();
    OscMessage myMessage = new OscMessage("/command");
    myMessage.add(millis());
    myMessage.add((float)random(1000));
    myMessage.add((float)random(1000));

    NetAddress myRemoteLocation= new NetAddress(remoteIP, remotePort);
    oscP5.send(myMessage, myRemoteLocation);
  }
}

void keyReleased() {
  if (key == 'U') { // be carfull
    NetAddress myRemoteLocation= new NetAddress(remoteIP, remotePort);
    println("firmware update");
    OscMessage myMessage = new OscMessage("/updatefirmware");
    oscP5.send(myMessage, myRemoteLocation);
  }
}

void mousePressed() {
  // sends message on mounse press
  OscMessage myMessage = new OscMessage("/click");
  myMessage.add(mouseX);
  myMessage.add(mouseY);
  NetAddress myRemoteLocation= new NetAddress(remoteIP, remotePort);
  oscP5.send(myMessage, myRemoteLocation);
}


void oscEvent(OscMessage theOscMessage) {
  if (theOscMessage.addrPattern().equals("/ping") == true) {
    int id = theOscMessage.get(1).intValue();
    String ip = theOscMessage.get(2).stringValue();
    String mac = theOscMessage.get(3).stringValue();
    float fw_version = theOscMessage.get(4).floatValue();
    println("got a ping from:");
    println(" id         : "+id);
    println(" ip         : "+ip);
    println(" mac        : "+mac);
    println(" fw_version : "+fw_version);
    remoteIP = ip; // we send some stuff back
  } else {
    print("### received an osc message.");
    print(" addrpattern: "+theOscMessage.addrPattern());
    println(" typetag: "+theOscMessage.typetag());
  }
}
