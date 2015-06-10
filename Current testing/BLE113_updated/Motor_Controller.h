#include "DRV8833_Lib.h"

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//
                                                      
#define         MOTOR_ANGLE_BUFFER      9             // Inputs with absolute value below this number are ignored
                                                      // the motor rotates 16 to 17 degrees additional angle after slow stopping.
                                                        
//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

MotorConfig theMotorConfig;

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//


//Moves motor to position (open, closed)
//receives either a "l" or a "u"
//Moves slightly closer to desiredAngle in desiredDirection
void controlMotor(char command)
{
  int motorActive = false;
  int desiredAngle = 0;
  int desiredDirection = theMotorConfig.turnDirection;
  float angleFromAccelerometer;
  
  if (command=='u' && isLocked==true) 
  {  desiredAngle = theMotorConfig.unlockedAngle; desiredDirection = theMotorConfig.turnDirection; motorActive = true;
      Serial.println("please unlock"); Serial.println(theMotorConfig.unlockedAngle);
  }
  else if (command=='l' && isLocked==false)
  {
    desiredAngle = theMotorConfig.lockedAngle; desiredDirection = -theMotorConfig.turnDirection; motorActive = true;
     Serial.println("please lock"); Serial.println(theMotorConfig.lockedAngle);
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
      Serial.println("keep driving");
      motorOut(theMotorConfig.power*desiredDirection);
    }
  }
  
}


void rotateMotor360()
{
  float initialAngleFromAccelerometerMinusBuffer = getAngle() - MOTOR_ANGLE_BUFFER;
  bool motorActive = true; 
  int desiredAngle = 0;
  int desiredDirection = theMotorConfig.turnDirection;
  float angleFromAccelerometer;
 
  while(motorActive == true)
  {
    angleFromAccelerometer = getAngle();
      Serial.println("current angle:");     Serial.println(angleFromAccelerometer);
      Serial.println("desired angle:");     Serial.println(initialAngleFromAccelerometerMinusBuffer);
        
    //Moves slightly closer to desiredAngle in desiredDirection
    if (abs(angleFromAccelerometer - initialAngleFromAccelerometerMinusBuffer) < MOTOR_ANGLE_BUFFER )
    {
      // reached position
         Serial.println("yay reached my destination");
      motorOut(0);
      delay(10000);
      motorActive = false;
      
    }
    else
    {
      Serial.println("keep driving");
      motorOut(140*desiredDirection);
      delay(2500);
      motorOut(0);
      delay(2500);
    }
  }
}
