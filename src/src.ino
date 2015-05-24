//**********************************************************************************************//
//					IMPORTS						 	//
//**********************************************************************************************//

// =============Arduino============
#import <Arduino.h>
#include <Wire.h>
// ============Memory============
#include "Driver_Memory.h"

// =============BLE============
#include <SoftwareSerial.h>
#include "Driver_BLE.h"

// =============Accelerometer============
#include "MPU6050_Lib.h"

// ============Motor============
#include "Driver_Motor.h"

// ============Execute USer Commands============
#include "Command.h"


//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

int statusLED = true;


//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//



//**********************************************************************************************//
//					SETUP AND LOOP					 	//
//**********************************************************************************************//


//The arduino runs the setup function first
void setup() 
{
//     Serial.println("start of loop1");
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  BLE_init();
  motor_init();
  //   Serial.println("start of loop2");
  accel_init();
    // Serial.println("start of loop3");
}



//the arduino keeps running the loop function until we shut it off
void loop() 
{
 //  Serial.println("start of loop4");
   delay(50); digitalWrite(13,statusLED); statusLED = !statusLED;
  
  // read commands sent by user
  readBLE();
   //  Serial.println("start of loop5");
  // execute the user commands
  executeCommandFromUser();

  // Serial.println("start of loop6");
  Serial.println(getAngle());
  
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


