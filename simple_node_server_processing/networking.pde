class Communication extends Thread {
  public void start() {
    super.start();
  }

  Communication() {
    println("Initilize Communication Thread");

    //exit();
  }
  public void run() {
    while (true) { // forever!!
      println(millis() + " running.");
      delay(1000);
    }
  }
}
