import oscP5.*;
import netP5.*;

Communication com;
OscP5 oscP5;


String remoteIP = "127.0.0.1";
int remotePort = 8888;
long lastsend = 0;

void setup() {
  size(800, 800);
  oscP5 = new OscP5(this, 9999);
  com = new Communication();
  com.start();
}


void draw() {
  background(0);
  if (millis()-lastsend>1000) {
    lastsend = millis();
    OscMessage myMessage = new OscMessage("/command1");
    myMessage.add(millis());
    myMessage.add((int)random(1000));
    NetAddress myRemoteLocation= new NetAddress(remoteIP, remotePort);
    oscP5.send(myMessage, myRemoteLocation);
  }
}

void mousePressed() {
  /* in the following different ways of creating osc messages are shown by example */
  OscMessage myMessage = new OscMessage("/command2");
  myMessage.add(123);  
  myMessage.add(234324.34);  
  myMessage.add(12.34);  
  NetAddress myRemoteLocation= new NetAddress(remoteIP, remotePort);
  oscP5.send(myMessage, myRemoteLocation);
}


/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: "+theOscMessage.addrPattern());
  println(" typetag: "+theOscMessage.typetag());
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
  }
}
