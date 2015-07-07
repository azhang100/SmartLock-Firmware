#define REFERENCE_VOLTAGE 6.6 //Double of 3.3 since the voltage is halved before sensing

float getBatteryVoltage(){
  return REFERENCE_VOLTAGE*analogRead(0)/1024;
}
