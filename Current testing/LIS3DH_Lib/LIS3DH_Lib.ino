#include <Wire.h>
#include <SoftwareSerial.h>
#include "LIS3DH_Lib.h"

float curAngle=0;
float prevAngle=0;
float coVar=1;

void setup(){
  Serial.begin(9600);
  accel_init();
}


void loop(){
  
  curAngle=getAngle();
  curAngle=filterData(&curAngle,&prevAngle,&coVar);
  Serial.println(curAngle);
  
  //Serial.println(getAngle());
  //getAngle();
  delay(10);
}
