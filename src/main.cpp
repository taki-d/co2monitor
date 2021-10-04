#include "Arduino.h"
#include "mh_z19c.h"

HardwareSerial serial(0);
mh_z19c co2sensor(2);

void setup() {
  serial.begin(9600);    
}

void loop() {
  serial.println(co2sensor.readPPM());
  sleep(1);
}


