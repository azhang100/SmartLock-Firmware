#include <Wire.h>
#include <SoftwareSerial.h>
#include "MPU6050_Lib.h"

void setup(){
  Serial.begin(9600);
  accel_init();
}


void loop(){
  Serial.println(getAngle());
  //getAngle();
  delay(10);
}
