#include <Arduino.h>

HardwareSerial serial(0);
HardwareSerial co2(2);

uint8_t read_co2[8] = {
  0xff,
  0x01,
  0x86,
  0x00,
  0x00,
  0x00,
  0x00
};

uint8_t self_cal_on[8] = {
  0xff,
  0x01,
  0x79,
  0xA0,
  0x00,
  0x00,
  0x00,
  0x00
};

uint8_t self_cal_off[8] = {
  0xff,
  0x01,
  0x79,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

uint8_t calcCheckSum(uint8_t *packet) {
  uint8_t checksum = 0;
  for (uint8_t i = 1; i < 8; i++){
    checksum += packet[i];
  }
  return 0xff - checksum + 0x01;
}



int readCO2(){
  co2.write(read_co2, 8);
  co2.write(calcCheckSum(read_co2));
  serial.println(calcCheckSum(read_co2), HEX);
  co2.flush();

  serial.println("reading");

  while (co2.available() != 0);

  uint8_t buf[9]; 
  co2.readBytes(buf, 9);

  int ppm = buf[3] | (buf[2] << 8);

  return ppm;
}

void setup() {
  co2.begin(9600);
  serial.begin(9600);    
}

void loop() {
  serial.println(readCO2());
  sleep(1);
}


