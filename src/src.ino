//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define           LOOP_TIME              10 // in milliseconds
#define           UPDATE_TIME            500 // in milliseconds
#define           SLEEP_TIME             10000 // in milliseconds

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//*****************************************************    *****************************************//

int statusLED = true;
int loopCount=0; // to slow down blinking

int loopsToUpdate = UPDATE_TIME/LOOP_TIME;
int sleepCount = 0;                   // counter
int loopsToSleep = SLEEP_TIME/LOOP_TIME;

//**********************************************************************************************//
//					IMPORTS						 	//
//**********************************************************************************************//

// =============Arduino============
#import <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// ============Debug==========
#include "debug.h"

// ============Memory============
#include <EEPROM.h>
#include "Driver_EEPROM.h"

// =============BT===========
#include "Driver_BLE113.h"
//#include "Driver_HC05.h"
//#include "Driver_BLESimSim.h"

// =============Accelerometer============
#include "Accel.h"

// ============Motor============
#include "Motor_Controller.h"

// ============Sleep============
#include <avr/sleep.h>
#include "Sleep.h"

// ============Security============
#include "Security.h"

// ============Execute User Commands============
#include "Command.h"

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
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);

  debug_init();
  
  debugPrintln("init accel...");
  accel_init(); // should be done as early as possible to give it as much time to calibrate
  debugPrintln("done");
  
  debugPrintln("init ble...");
  BLE_init();
  debugPrintln("done");
  
  debugPrintln("init motor...");
  motor_init();
  debugPrintln("motor hard test");
  motorTime(200);
  delay(100);
  motorTime(-200);
  debugPrintln("done");
  
  debugPrintln("setup done");
}


void loop()
{
  // UPDATE
  delay(LOOP_TIME);
  loopCount++;
  sleepCount++;
  int angle = getAngle();
  //update_motor();
  
  if (loopCount == loopsToUpdate){
    loopCount = 0;
    statusLED = !statusLED;
    digitalWrite(13,statusLED);
    debugPrint("Current Orientation: "); debugPrintlnInt(angle);
    // debugPrint("Awake for: "); debugPrint(sleepCount); debugPrintlnInt(" loops");
  }

  if (sleepCount >= loopsToSleep){sleep();}

   // CONTROL
  
  String command; // read commands sent by user
  command=readBLE();
  recievedCommand(command);
  command=readSerial();
  recievedCommand(command);
}

void recievedCommand(String command){
  if (command.length() > 0){
    debugPrint("Got Command: ");debugPrintln(command);
    // execute the user commands
    executeCommandFromUser(command);
    debugCommandFromUser(command);
    resetSleepCounter();
  }
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//
