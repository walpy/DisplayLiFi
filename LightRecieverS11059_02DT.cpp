
// Copyright (c) 2015 IKEUCHI Yasuki

#include "LightRecieverS11059_02DT.h"
#include <Arduino.h>
#include <Wire.h>

#define DEVICE_ADDRESS 0x2A
#define CONTROL_REG 0x00
#define TIMING_REG_H  0x01
#define TIMING_REG_L 0x02
#define THRESHOLD 9

enum PHYSICAL_LAYER {
  FLAG,
  DATA
};


unsigned int physical_status = FLAG;

LightRecieverS11059_02DT::LightRecieverS11059_02DT()
{
  Wire.begin();
}

void LightRecieverS11059_02DT::getRGB()
{
  uint16_t colordata = 0;

  //manual exposure mode  

  //preset gain mode exposure time (each color ch)
  //  00 = 87.5us
  //  01= 1.4ms
  //  10= 22.4ms
  //  11= 179.2ms 

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(CONTROL_REG);
  //Wire.write(0x84); // ADC reset LowGain 87.5us wakeup
  Wire.write(0x85); // ADC reset LowGain 1.4ms wakeup
  //Wire.write(0x86); // ADC reset LowGain 22.4ms wakeup
  //Wire.write(0x8C); // ADC reset HighGain 87.5us wakeup
  Wire.endTransmission(false);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(TIMING_REG_H);
  Wire.write(0);
  Wire.endTransmission(false);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(TIMING_REG_L);
  Wire.write(0x80);
  Wire.endTransmission(false);

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(CONTROL_REG);
  Wire.write(0x04); //start
  Wire.endTransmission(true); 

  uint16_t end_measuring = 0;
  while(!end_measuring) {
    Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(CONTROL_REG);
    Wire.endTransmission(false);
    Wire.requestFrom(0x2A,1, false);
    uint16_t control_reg = Wire.read();
    end_measuring = control_reg & 0x20; 
    delay(1);
  }

  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(0x2A,8,true);

  //read data
  colordata = Wire.read();
  colordata <<= 8;
  colordata |= Wire.read();
  red = colordata;
  colordata =0;
  colordata = Wire.read();
  colordata <<= 8;
  colordata |= Wire.read();
  green = colordata;
  colordata =0;
  colordata = Wire.read();
  colordata <<= 8;
  colordata |= Wire.read();
  blue = colordata;
  colordata =0;
  colordata = Wire.read();
  colordata <<= 8;
  colordata |= Wire.read();
  ir = colordata;

  return;
}

#ifdef DISPLAYLIFI_TRACE
void LightRecieverS11059_02DT::printRGB() {
  Serial.print(max_red >= THRESHOLD ? "1" : "0");
  Serial.print(max_green >= THRESHOLD ? "1" : "0");
  Serial.print(max_blue >= THRESHOLD ? "1" : "0");
  Serial.print(": (");

  Serial.print(max_red);
  Serial.print(',');
  Serial.print(max_green);
  Serial.print(',');
  Serial.print(max_blue);
  Serial.println(')');
}
#endif

void LightRecieverS11059_02DT::loop()
{
  getRGB();

  if (red >= 1 || green >= 1 || blue >= 1) {
    physical_status = DATA;
    max_red = max(max_red, red);
    max_green = max(max_green, green);
    max_blue = max(max_blue, blue);
  } 
  else {
#ifdef DISPLAYLIFI_TRACE
    if (physical_status == DATA) {
      printRGB();
    }
#endif
    physical_status = FLAG;

    max_red = 0;
    max_green = 0;
    max_blue = 0;
  }
}

