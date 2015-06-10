#include <Wire.h>
#include <SoftwareSerial.h>
//#include "LIS3DH_Lib.h"

#define     SLAVE_ADDR     0x18
#define     CTRL_REG1      0x20
#define     CTRL_REG1_VAL  0b01000111  //50Hz data rate and xyz axis enabled with in normal mode

float curAngle=0;
float prevAngle=0;
float coVar=1;


void setup(){
  Serial.begin(9600);
  Wire.begin();
        Wire.beginTransmission(SLAVE_ADDR);      //transmission with this address
        Wire.write(CTRL_REG1);          //first specifies power management address of MCU
        Wire.write(CTRL_REG1_VAL);               //awakes MCU by sending 0 to above register address
        Wire.endTransmission(true);
  //accel_init();
}


void loop(){
    int data=0;
    
    Wire.beginTransmission(SLAVE_ADDR);     //starting the communication again
    Wire.write(0x0F);//(0x28 | 0x80);              //start with this register address (its the first data register
    //Wire.endTransmission(false);          //continue to read data
    //Wire.requestFrom(SLAVE_ADDR,2,true);      //request the slave to send the 2 byte data
    Serial.println(Wire.read());
    //data=(Wire.read()<<8)|Wire.read();      //data is 16 bit data .the data is automatically read sequently
    Wire.endTransmission();
  delay(100);
}
