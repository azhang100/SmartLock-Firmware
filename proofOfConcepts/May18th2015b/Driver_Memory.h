//#include <EEPROM.h>

// All the settings should be stored here
// And all the memory-related stuff

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

//EEPROM.read(address);
//EEPROM.write(address, val);

bool isLocked=false;

class MotorConfig
{
  
  public:
    int turnDirection;
    float unlockedAngle;
    float lockedAngle;
    int power; // ranges from 10 to 100
    bool isMotorConfigured;
    
    bool isTurnDirectionSet;
    bool isUnlockedAngleSet;
    bool isLockedAngleSet;
    bool isPowerSet; 
    
    MotorConfig()
    {
      turnDirection = -1; // clockwise turn to lock, anti clockwise turn to unlock
      unlockedAngle = 180;
      lockedAngle = 70;
      power = 255; // ranges from 50 to 255
      isMotorConfigured = false;
      isTurnDirectionSet = false;
      isUnlockedAngleSet = false;
      isLockedAngleSet = false;
      isPowerSet = false; 
    }
    
    void setTurnDirection(int dir)
    {
      turnDirection = dir;
      isTurnDirectionSet = true;
      isMotorConfigured = isTurnDirectionSet && isUnlockedAngleSet && isLockedAngleSet && isPowerSet;
    }
    
    void setUnlockedAngle(float angle)
    {
      unlockedAngle = angle;
      isUnlockedAngleSet = true;
      isMotorConfigured = isTurnDirectionSet && isUnlockedAngleSet && isLockedAngleSet && isPowerSet;
    }
    
    void setLockedAngle(float angle)
    {
      lockedAngle = angle;
      isLockedAngleSet = true;
      isMotorConfigured = isTurnDirectionSet && isUnlockedAngleSet && isLockedAngleSet && isPowerSet;
    }
    
    void setPower(int apower)
    {
      power = apower;
      isPowerSet = true;
      isMotorConfigured = isTurnDirectionSet && isUnlockedAngleSet && isLockedAngleSet && isPowerSet;
    }

};



