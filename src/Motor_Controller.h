#include "DRV8833_Lib.h"

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//
                                                      
#define         MOTOR_ANGLE_BUFFER      9             // Inputs with absolute value below this number are ignored
                                                      // the motor rotates 16 to 17 degrees additional angle after slow stopping.
                                                        
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
      debugSerial.println("please unlock"); debugSerial.println(settings.unlockedAngle.getData());
  }
  else if (command=='l' && isLocked==false)
  {
    desiredAngle = settings.lockedAngle.getData(); desiredDirection = -settings.turnDirection.getData(); motorActive = true;
     debugSerial.println("please lock"); debugSerial.println(settings.lockedAngle.getData());
  }  
  
  while(motorActive == true)
  {
    angleFromAccelerometer = getAngle();
      debugSerial.println("current angle:");     debugSerial.println(angleFromAccelerometer);
      debugSerial.println("desired angle:");     debugSerial.println(desiredAngle);
        
    //Moves slightly closer to desiredAngle in desiredDirection
    if (abs(angleFromAccelerometer - desiredAngle) < MOTOR_ANGLE_BUFFER )
    {
      // reached position
         debugSerial.println("yay reached my destination");
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
      debugSerial.println("keep driving");
      motorOut(settings.power.getData()*desiredDirection);
    }
  }
}
