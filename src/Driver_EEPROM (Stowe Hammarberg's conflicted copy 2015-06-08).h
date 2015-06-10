// All the settings should be stored here
// And all the memory-related stuff

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define     MASTER_MEMORY_ADDRESS        0     // this is the address of a bool.  
                                               // The bool is "have the settings ever been set to"
                                               // The purpose is to have a default settings


//**********************************************************************************************//
//					CLASSES          				 	//
//**********************************************************************************************//

class Attribute
{
  byte address;
  byte data;
  
  public:
    
    Attribute(){}
    
    byte getData(){return data;}
    
    void readData(){data = EEPROM.read(address);}
    
    void setData(byte dataIn){
      data = dataIn;
      EEPROM.write(address,data);
    }
    
    void setAddress(byte addressIn){
      address = addressIn;
      EEPROM.write(address,data);
    }
};

class Settings
{
  public:
    Attribute turnDirection;   turnDirection.setAddress(1);
    Attribute unlockedAngle;   unlockedAngle.setAddress(2);
    Attribute lockedAngle;     lockedAngle.setAddress(3);
    Attribute power;           power.setAddress(4); // ranges from 50 to 255
   
    Settings()
    {
      set = EEPROM.read(MASTER_MEMORY_ADDRESS);
      if (set == 0){
          turnDirection.setData(1);
          unlockedAngle.setData(180);
          lockedAngle.setData(70);
          power.setData(255);
          EEPROM.write(MASTER_MEMORY_ADDRESS, 1); // so next time it just reads the data
      }
      else { // (set == 1)
          turnDirection.readData();
          unlockedAngle.readData();
          lockedAngle.readData();
          power.readData();
      }
    }
};

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

bool isLocked=false;
bool commandAvailable = false;

//**********************************************************************************************//
//					FUNCTIONS        				 	//
//**********************************************************************************************//

