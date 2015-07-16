#include "Driver_DRV8833.h"

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
  
  if (command=='u') 
  {  desiredAngle = settings.unlockedAngle.getData(); desiredDirection = settings.turnDirection.getData(); motorActive = true;
      debugPrintln("please unlock"); debugPrintlnInt(settings.unlockedAngle.getData());
  }
  else if (command=='l')
  {
    desiredAngle = settings.lockedAngle.getData(); desiredDirection = -settings.turnDirection.getData(); motorActive = true;
     debugPrintln("please lock"); debugPrintlnInt(settings.lockedAngle.getData());
  }  
  
  while(motorActive == true)
  {
    angleFromAccelerometer = getAngle();
      debugPrintln("current angle:");     debugPrintlnInt(angleFromAccelerometer);
      debugPrintln("desired angle:");     debugPrintlnInt(desiredAngle);
        
    //Moves slightly closer to desiredAngle in desiredDirection
    if (abs(angleFromAccelerometer - desiredAngle) < MOTOR_ANGLE_BUFFER )
    {
      // reached position
         debugPrintln("yay reached my destination");
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
      // debugPrintln("keep driving");
      motorOut(settings.power.getData()*desiredDirection);
    }
  }
}

void motorTime(int time){
  if (time == 0) {
    motorOut(0);
  }
  else if (time > 0) {
    motorOut(255);
    delay(time);
    motorOut(0);
  }
  else if (time < 0) {
    motorOut(-255);
    delay(-time);
    motorOut(0);
  }
}
