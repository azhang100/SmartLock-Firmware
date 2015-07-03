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

SoftwareSerial debugSerial(0,1);

// ============Memory============
#include <EEPROM.h>
#include "Driver_EEPROM.h"

// =============BT===========
//#include "Driver_BLE113.h"
# include "Driver_HC05.h"

// =============Accelerometer============
//#include "MPU6050_Lib.h"
#include "LIS3DH_Lib.h"

// ============Motor============
#include "Motor_Controller.h"

// ============Sleep============
#include <avr/sleep.h>
#include "Driver_Sleep.h"

// ============Execute USer Commands============
#include "Command.h"

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//*****************************************************    *****************************************//

int statusLED = true;
int loopCount=0; // to slow down blinking

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void setup();
void loop();

//**********************************************************************************************//
//					SETUP AND LOOP					 	//
//**********************************************************************************************//

//The arduino runs the setup function first
void setup() 
{
  debugSerial.begin(9600);
  debugSerial.println("start of setup");
  
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  
  debugSerial.println("init accel...");
  accel_init(); // should be done as early as possible to give it as much time to calibrate
  debugSerial.println("done");
  
  debugSerial.println("init ble...");
  BLE_init();
  debugSerial.println("done");
  
  debugSerial.println("init motor...");
  motor_init();
  debugSerial.println("done");
  
  debugSerial.println("setup done");
}


void loop()
{
  delay(1);
  loopCount++;
  
  if (loopCount == 200){
    sleepCount++;
    loopCount = 0;
    statusLED = !statusLED;
    digitalWrite(13,statusLED);
    debugSerial.print("Current Orientation: ");debugSerial.println(getAngle());
    debugSerial.print("Awake for: "); debugSerial.print(sleepCount); debugSerial.println(" loops");
  }
  
  String command; // read commands sent by user
  command=readBLE();
  recievedCommand(command);
  
  if (sleepCount >= sleepCountMax){sleep();}
  
}

void recievedCommand(String command){
  if (command.length() > 0){
    debugSerial.print("Got Command: ");debugSerial.println(command);
    // execute the user commands
    executeCommandFromUser(command);
    debugCommandFromUser(command);
    resetSleepCounter();
  }
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


