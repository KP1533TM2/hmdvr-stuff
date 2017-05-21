#include <Wire.h>

uint8_t tx_buf[2048] = {0};
uint8_t rx_buf[2048] = {0};

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  while(!Serial);
}

uint16_t tvp5150_read(uint8_t addr, uint16_t len, uint8_t *rx_buf) {
  uint16_t bytes_received = 0;
  uint8_t a = addr;
  for(uint16_t i = 0; i < len; i++) {
    Wire.beginTransmission(0xBB>>1);
    Wire.write(a++);
    Wire.endTransmission();
    Wire.requestFrom(0xBB>>1, 1);
    while(Wire.available()) {
      (*rx_buf++) = Wire.read();
      bytes_received++;
    }
  }
  return bytes_received;
}

uint16_t tvp5150_write(uint8_t addr, uint16_t len, uint8_t *tx_buf, bool autoincrement) {
  uint16_t bytes_transmitted = 0;
  uint8_t a = addr;
  for(uint16_t i = 0; i < len; i++) {
    Wire.beginTransmission(0xBA>>1);
    Wire.write(a);
    a = autoincrement?a+1:a;
    Wire.write(*tx_buf++);
    Wire.endTransmission();
    bytes_transmitted++;
  }
  return bytes_transmitted;
}

uint8_t state = 0;
uint8_t addr  = 0;
uint8_t len   = 0;
uint8_t count = 0;
uint8_t incr  = 0;
void loop() {

  while(Serial.available() > 0) {
    switch(state) {
      case 0: {
        switch(Serial.read()) {
          case 'r': state = 0x80; break;
          case 'w': state = 0x40; break;
          default: break;
        }
        break;
      }
      // WRITE
      case 0x40: {
        addr = Serial.read();
        state = 0x41;
        break;
      }
      case 0x41: {
        len = Serial.read();
        state = 0x42;
        break;
      }
      case 0x42: {
        incr = Serial.read();
        state = 0x43;
        break;
      }
      case 0x43: {
        if(count < len) {
          tx_buf[count++] = Serial.read();
        } else {
          state = 0x44;
          count = 0;
          Serial.write(0x55);
        }
        break;
      }
      case 0x44: {
        state = 0x00;
        if(Serial.read() != '\n') break;
        //Serial.println(Serial.read(), DEC);
        tvp5150_write(addr, len, tx_buf, incr?true:false);
        break;
      }
      // READ
      case 0x80: {
        addr = Serial.read();
        state = 0x81;
        break;
      }
      case 0x81: {
        len = Serial.read();
        state = 0x82;
        break;
      }
      case 0x82: {
        state = 0x00;
        if(Serial.read() != '\n') break;
        uint16_t rcvd = tvp5150_read(addr, len, rx_buf);
        for (uint16_t i = 0; i < rcvd; i++) {
          Serial.write(rx_buf[i]);
        }
        break;
      }
    }
  }
/*  
  uint16_t rcvd = tvp5150_read(0x80, 2, rx_buf); 
  
  Serial.print("bytes rcvd: ");
  Serial.println(rcvd);

  for(uint16_t i = 0; i < rcvd; i++) {
    uint16_t ddd = rx_buf[i];
    Serial.print((uint8_t)ddd, HEX);
    Serial.print(" ");
  }
  
  while(1);*/
}
