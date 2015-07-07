#include "DRV8833_Lib.h"

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//
                                                      
#define         MOTOR_ANGLE_BUFFER      10             // the motor rotates 16 to 17 degrees additional angle after slow stopping.
                                                        
//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//


//Moves motor to position (open, closed)
//receives either a "l" or a "u"
//Moves slightly closer to desiredAngle in desiredDirection
void controlMotor(char command){
  int motorActive = false;
  int desiredAngle = 0;
  int desiredDirection = settings.turnDirection.getData();
  float angleFromAccelerometer;
  
  if (command=='u' && isLocked==true) 
  {  desiredAngle = settings.unlockedAngle.getData(); desiredDirection = settings.turnDirection.getData(); motorActive = true;
      Serial.println("please unlock"); Serial.println(settings.unlockedAngle.getData());
  }
  else if (command=='l' && isLocked==false)
  {
    desiredAngle = settings.lockedAngle.getData(); desiredDirection = -settings.turnDirection.getData(); motorActive = true;
     Serial.println("please lock"); Serial.println(settings.lockedAngle.getData());
  }  
  
  while(motorActive == true)
  {
    angleFromAccelerometer = getAngle();
      Serial.println("current angle:");     Serial.println(angleFromAccelerometer);
      Serial.println("desired angle:");     Serial.println(desiredAngle);
        
    //Moves slightly closer to desiredAngle in desiredDirection
    if (abs(angleFromAccelerometer - desiredAngle) < MOTOR_ANGLE_BUFFER )
    {
      // reached position
         Serial.println("yay reached my destination");
      motorOut(0);
      motorActive = false;
       if (command=='u') 
       {  
         isLocked=false;
       }
       else if (command=='l')
       {
         isLocked=true;
       }  
    }
    else
    {
      // Serial.println("keep driving");
      motorOut(settings.power.getData()*desiredDirection);
    }
  }
}
