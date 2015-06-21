#include "arduino.h"

#include <Wire.h>
#include "LIS3DH_Lib.h"

void setup(){
  Serial.begin(9600);
  Serial.println("Serial active");
  accel_init();
  Serial.println("setup done");
}

void loop(){
  Serial.println(getAngle());
}
