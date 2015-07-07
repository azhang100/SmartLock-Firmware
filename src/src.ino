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

// SoftwareSerial debugSerial(0,1);

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


void loop()
{
  // UPDATE
  delay(LOOP_TIME);
  loopCount++;
  sleepCount++;
  int angle = getAngle();
  update_motor();
  
  if (loopCount == loopsToUpdate){
    loopCount = 0;
    statusLED = !statusLED;
    digitalWrite(13,statusLED);
    Serial.print("Current Orientation: "); Serial.println(angle);
    //Serial.print("Awake for: "); Serial.print(sleepCount); Serial.println(" loops");
  }

  if (sleepCount >= loopsToSleep){sleep();}

  // CONTROL
  
  String command; // read commands sent by user
  command=readBLE();
  recievedCommand(command);
}

void recievedCommand(String command){
  if (command.length() > 0){
    Serial.print("Got Command: ");Serial.println(command);
    // execute the user commands
    executeCommandFromUser(command);
    debugCommandFromUser(command);
    resetSleepCounter();
  }
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//


