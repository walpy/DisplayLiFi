/******************************************************************************** 
 * S11059_02dtS11059_02dtS11059-02DT I2C color sensor test sketch  v20130818   by KentN 
 *
 * http://www.hamamatsu.com/jp/ja/product/new/S11059-02DT/index.html  (Ja 
 * http://www.hamamatsu.com/jp/en/product/new/S11059-02DT/index.html  (EN
 * 
 *  tested boards  
 *  MSP430 LaunchPad(G2553) (Energia IDE 0101E009 & patched latest　MSP430/cores library) 
 *   Arduino 1.0 or later (need 3.3V board or I2C level converter) 
 * 
 *  MSP430LaunchPad (& Energia) 
 * before enabling I2C bus, remove led2 JUMPER PIN 
 * Energia IDE must be patched with latest cores library on github 
 * (In energiaIDEdirectory/hardware/MSP430/cores) 
 ********************************************************************************/

#include "Wire.h"

#define LED 13 // RED_LED  //(MSP430 LaunchPad)

#define DEVICE_ADDRESS 0x2A
#define CONTROL_REG 0x00
#define TIMING_REG_H  0x01
#define TIMING_REG_L 0x02
#define THRESHOLD 9

enum PHYSICAL_LAYER {
  FLAG,
  DATA
};

uint16_t red = 0;
uint16_t green = 0;
uint16_t blue = 0;
uint16_t ir = 0;

uint16_t max_red = 0;
uint16_t max_green = 0;
uint16_t max_blue = 0;

unsigned int physical_status = FLAG;

void getRGB();

void setup()
{
  Wire.begin();
  Serial.begin(9600); 
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
}

void loop()
{

  getRGB();

  /*printRawRGB();
  led_blink();
  return;*/

  if (red >= 1 || green >= 1 || blue >= 1) {
    switch (physical_status) {
    case FLAG:
      //printRGB();
      break;
    case DATA:
    default:
      break;
    }
    physical_status = DATA;
    max_red = max(max_red, red);
    max_green = max(max_green, green);
    max_blue = max(max_blue, blue);
  } 
  else {
    if (physical_status == DATA) {
      printRGB();
    }
    physical_status = FLAG;

    max_red = 0;
    max_green = 0;
    max_blue = 0;
  }

  //delay(500);
  //delay(10);
}

void printRGB() {
  Serial.print(max_red >= THRESHOLD ? "1" : "0");
  Serial.print(max_green >= THRESHOLD ? "1" : "0");
  Serial.print(max_blue >= THRESHOLD ? "1" : "0");
  Serial.print(':');

  Serial.print(max_red);
  Serial.print(',');
  Serial.print(max_green);
  Serial.print(',');
  Serial.println(max_blue);
}

void printRawRGB() {
  Serial.print(red);
  Serial.print(',');
  Serial.print(green);
  Serial.print(',');
  Serial.print(blue);
  Serial.print(',');
  Serial.println(ir);
}

void getRGB()
{
  uint16_t colordata = 0;

  //preset gain mode exposure time (each color ch)
  //  00 = 87.5us
  //  01= 1.4ms
  //  10= 22.4ms
  //  11= 179.2ms 


  //manual exposure mode  
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
  //Wire.write(0x40);
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

  /*
  // Static mode
   Wire.beginTransmission(DEVICE_ADDRESS);
   Wire.write(CONTROL_REG);
   // Wire.write(0x8A); // ADC reset wakeup 22.4ms
   Wire.write(0x89); // ADC reset wakeup 1.4ms
   Wire.endTransmission(false);
   Wire.beginTransmission(DEVICE_ADDRESS);
   Wire.write(CONTROL_REG);
   Wire.write(0x09);
   Wire.endTransmission(true);
   */
  //delay(23*4); //total exposure time
  //delay(0);

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

void setManualtGain(uint16_t ex_time){
  uint8_t reg =0;
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(TIMING_REG_H);
  reg = (ex_time >> 8);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.beginTransmission(DEVICE_ADDRESS);
  Wire.write(TIMING_REG_L);
  reg |= ex_time;
  Wire.write(reg);
  Wire.endTransmission(false);
}

void led_blink()
{
  digitalWrite(LED,HIGH);
  delay(10);
  digitalWrite(LED,LOW);
}




