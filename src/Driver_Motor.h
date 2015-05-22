
#include "MPU6050_Lib.h"
//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

// wired connections
#define HG7881_B_IA 9 // D11 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 11 // D12 --> Motor B Input B --> MOTOR B -
 
// functional connections
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction

#define         MOTOR_DUTY_BUFFER       80            // Inputs with absolute value below this number are ignored
                                                      // Anyways the motor can't anything worthwhile with such less power
                                                      
#define         MOTOR_ANGLE_BUFFER      5             // Inputs with absolute value below this number are ignored
                                                      // the motor rotates 16 to 17 degrees additional angle after slow stopping.
                                                        
//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

MotorConfig theMotorConfig;

int desiredAngle = 0;
int desiredDirection = theMotorConfig.turnDirection;
int motorActive = false;

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//


void motorOut(int rotationalVelocity);  //Sets motor rotational velocity


//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//


// Motor initialization
void Motor_init()			         
{
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
}



//Moves motor to position (open, closed)
//receives either a "l" or a "u"
//Moves slightly closer to desiredAngle in desiredDirection
void controlMotor(char command)
{
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



// actual motor driver function
void motorOut(int rotationalVelocity){
  int dutyCycle = rotationalVelocity;
  
  if (dutyCycle > MOTOR_DUTY_BUFFER)
  {analogWrite(MOTOR_B_PWM, dutyCycle);analogWrite(MOTOR_B_DIR, 0);Serial.println("forwards");}
  
  else if (dutyCycle < -MOTOR_DUTY_BUFFER)
  {analogWrite(MOTOR_B_DIR, -dutyCycle);analogWrite(MOTOR_B_PWM, 0);Serial.println("backwards");}
  
  else
  {analogWrite(MOTOR_B_PWM, 0);analogWrite(MOTOR_B_DIR, 0);Serial.println("stop");}
}
