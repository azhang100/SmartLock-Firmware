//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					IMPORTS						 	//
//**********************************************************************************************//

// =============Arduino============
#import <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ============Memory============
#include "Driver_Memory.h"

// =============BLE============
// =============Arduino============
#import <Arduino.h>
#include <Wire.h>
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
int loopCount=0; // to slow down blinking

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					SETUP AND LOOP					 	//
//**********************************************************************************************//

//The arduino runs the setup function first
void setup() 
{
  Serial.begin(9600);
  Serial.println("start of setup");
  
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  
  Serial.println("init accel...");
  accel_init(); // should be done as early as possible to give it as much time to calibrate
  Serial.println("done");
  
  Serial.println("init ble...");
  BLE_init();
  Serial.println("done");
  
  Serial.println("init motor...");
  motor_init();
  Serial.println("done");
  
  Serial.println("setup done");
}

//the arduino keeps running the loop function until we shut it off
void loop()
{
  delay(1);
  loopCount = loopCount + 1;
  if (loopCount == 500){
    loopCount = 0;
    statusLED = !statusLED;
    digitalWrite(13,statusLED);
    Serial.print("Current Orientation: ");Serial.println(getAngle());
  }
  
  // read commands sent by user
  String command=readBLE();
  if (command.length() > 0){
    Serial.print("Got Command: ");Serial.println(command);
    // execute the user commands
    executeCommandFromUser(command);
  }
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


