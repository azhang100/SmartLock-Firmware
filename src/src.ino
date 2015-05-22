//**********************************************************************************************//
//					IMPORTS						 	//
//**********************************************************************************************//

// =============Arduino============
#import <Arduino.h>

// =============BLE============
#include <SoftwareSerial.h>
#include "Driver_BLE.h"

// ============Memory============
#include "Driver_Memory.h"

// ============Accelerometer============

#include "Wire.h"
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

void readBLE();


//**********************************************************************************************//
//					SETUP AND LOOP					 	//
//**********************************************************************************************//


//The arduino runs the setup function first
void setup() 
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  BLE_init();
  Motor_init();
  accel_init();
}



//the arduino keeps running the loop function until we shut it off
void loop() 
{
  // delay(50); digitalWrite(13,statusLED); statusLED = !statusLED;
  
  // read commands sent by user
  readBLE();
  
  // execute the user commands
  executeCommandFromUser();
  
  //Serial.println(getAngle());
  
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


void readBLE() {

  while (BLE.available()) {
    // get the new byte:
    char inChar = (char)BLE.read(); 
    //Serial.write(inChar);
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a semi colon, set a flag
    // so the main loop can do something about it:
    if (inChar == ';') {
//      int pos = inputString.indexOf(';');
//      inputString[pos+1] = '\n';
      stringComplete = true;
      Serial.println(inputString);
    } 
  }
  
}

