// All the settings should be stored here
// And all the memory-related stuff

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define     MASTER_MEMORY_ADDRESS        0     // this is the address of a bool.  
                                               // The bool is "have the settings ever been set to"
                                               // The purpose is to have a default settings
#define     DEFAULT_EEPROM_VAL           255   // Default EEPROM value, 255 when shipping atmega

//**********************************************************************************************//
//					CLASSES          				 	//
//**********************************************************************************************//

class Attribute
{
  int address;
  byte data;
  
  public:
    
    Attribute(){
      address =0;
      data=0;
    }
    
    byte getData(){return data;}
    
    void readData(){data = EEPROM.read(address);}
    
    void setData(byte dataIn){
      data = dataIn;
      EEPROM.write(address,data);
    }
    
    void setAddress(byte addressIn){
      address = addressIn;
    }
};

class Settings
{
  public:
    Attribute turnDirection;   
    Attribute unlockedAngle;   
    Attribute lockedAngle;     
    Attribute power;            // ranges from 50 to 255
    Attribute turnTestTime;
   
    Settings()
    {
      turnDirection.setAddress(1);
      unlockedAngle.setAddress(2);
      lockedAngle.setAddress(3);
      power.setAddress(4);
      turnTestTime.setAddress(5);
      
      int set = EEPROM.read(MASTER_MEMORY_ADDRESS);
      if (set == DEFAULT_EEPROM_VAL){
          turnDirection.setData(1);
          unlockedAngle.setData(180);
          lockedAngle.setData(70);
          turnTestTime.setData(200);
          power.setData(255);
          EEPROM.write(MASTER_MEMORY_ADDRESS, 1); // so next time it just reads the data
      }
      else { // (set =! DEFAULT_EEPROM_VAL)
          turnDirection.readData();
          unlockedAngle.readData();
          lockedAngle.readData();
          power.readData();
          turnTestTime.readData();
      }
    }
};

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

bool isLocked=false;
bool commandAvailable = false;

Settings settings;

//**********************************************************************************************//
//					FUNCTIONS        				 	//
//**********************************************************************************************//

