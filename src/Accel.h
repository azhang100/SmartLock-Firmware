//**********************************************************************************************//
//          CLASSES                   //
//**********************************************************************************************//

class FilteredVariable
{
  float newValueWeight;
  float lastCalled;
  
  public:
    float value;
    float differential; // difference in value per time
    
    FilteredVariable(){
      newValueWeight = 0.2;
      value=0;
      differential=0;
      lastCalled=0;
    }
    
  float filterData(float newValue){//float prevAngle, float prevVelocity, float newAngle, float newAngleWeight
  
    float currentTime = millis();
    float deltaT = currentTime - lastCalled; lastCalled = currentTime;
  
    value = value + differential*deltaT; // adjust prevAngle for change
    float newDifferential = newValue - value; // its velocity per frequency at which this function is called
    differential = (differential*(1.0-newValueWeight) + newDifferential*newValueWeight)/deltaT;
    value = value*(1.0-newValueWeight) + newValue*newValueWeight; // just a weighted average
    return value;
  }
};

struct AccelData {
  int outX, outY, outZ; 
  FilteredVariable xVal;
  FilteredVariable yVal;
  FilteredVariable zVal;
};

AccelData accelData;

//**********************************************************************************************//
//          IMPORTS             //
//**********************************************************************************************//

//#include "Driver_MPU6050.h"
//#include "Driver_LIS3DH.h"
#include "Driver_noAccel.h"

//**********************************************************************************************//
//           FUNCTIONS DECLARATIONS         //
//**********************************************************************************************//

int getAngle();  //uses getData() to return the angle

//**********************************************************************************************//
//          FUNCTIONS           //
//**********************************************************************************************//

int getAngle(){
  getData();
  accelData.xVal.filterData(accelData.outX);
  accelData.yVal.filterData(accelData.outY);
  accelData.zVal.filterData(accelData.outZ);
  int angle=(180*atan2(accelData.xVal.value,accelData.zVal.value)/PI);
  angle = angle + 180;
  angle = map(angle,0,360,0,255);
  return angle;
}
