//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					IMPORTS						 	//
//**********************************************************************************************//

// =============Arduino============
#import <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h> // debugging
SoftwareSerial debug(4, 5); // RX, TX

// ============Memory============
#include "Driver_Memory.h"

// =============BLE============
#include "Driver_BLE.h"

// =============Accelerometer============
#include "LIS3DH_Lib.h"

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
  debug.begin(9600);
  debug.println("start of setup");
  
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  
  debug.print("init accel...");
  accel_init(); // should be done as early as possible to give it as much time to calibrate
  debug.print("done");
  
  debug.print("init ble...");
  BLE_init();
  debug.print("done");
  
  debug.print("init motor...");
  motor_init();
  debug.print("done");
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
    debug.print("Current Orientation: ");debug.println(getAngle());
  }
  
  // read commands sent by user
  String command=readBLE();
   
  // execute the user commands
  executeCommandFromUser(command);
  
  int i=0;

  i=getData(0x28);
  debug.println(i);
  i=getData(0x2A);
  debug.println(i);
  i=getData(0x2C);
  debug.println(i);
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


