//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define		BAUD_RATE          9600

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
#include "Motor_Controller.h"

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
//     Serial.println("start of setup");
  pinMode(13,OUTPUT);
  Serial.begin(BAUD_RATE);
  BLE_init();
  motor_init();
  //   Serial.println("after motor init");
  accel_init();
    // Serial.println("after accel init");
}



//the arduino keeps running the loop function until we shut it off
void loop() 
{
  //  Serial.println("start of loop");
  delay(50); digitalWrite(13,statusLED); statusLED = !statusLED;
  
  // read commands sent by user
  readBLE();
  //  Serial.println("after readBLE");
   
  // execute the user commands
  executeCommandFromUser();


  // Serial.println("end of loop");
  Serial.println(getAngle());
  
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


