//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define           LOOP_TIME              10 // in milliseconds
#define           UPDATE_TIME            500 // in milliseconds
#define           SLEEP_TIME             ((long) 10000*100) // in milliseconds
//#define           SLEEP_TIME             (long) 10000 // in milliseconds

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//*****************************************************    *****************************************//

int statusLED = true;
int loopCount=0; // to slow down blinking

int loopsToUpdate = UPDATE_TIME/LOOP_TIME;
int sleepCount = 0;                   // counter
long loopsToSleep = SLEEP_TIME/LOOP_TIME;

static const PROGMEM char * const SPACE = " ";
//**********************************************************************************************//
//					IMPORTS						 	//
//**********************************************************************************************//

// =============Arduino============
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "Config.h"
#include "Test.h"
// ============Debug==========
#include "Debug.h"

// ============Memory============
#include <EEPROM.h>
#include "Driver_EEPROM.h"

// =============BT===========
#ifdef HAVE_BLE
#include "Driver_BLE113.h"
BLE gBLE(BLE_RX, BLE_TX);
#endif

#include "Battery_config.h"
#
#ifdef DO_BATTERY
#include "BatterySubject.h"
#include "BatteryObserver.h"

BatterySubject gBattSubj;
BatteryObserver gBattObs(&gBattSubj);
#endif

// =============Accelerometer============
//#include "Accel.h"
//#include "LIS3DH_Subject.h"
#include "STMACC_Subject.h"
#include "LockAccelerometerObserver.h"

STMACC_Subject gAccelerometerSubject(STMACC_Subject::LIS3DH);
LockAccelerometerObserver gLockAccelerometerObserver(&gAccelerometerSubject);

// ============Motor============
//#include "Motor_Controller.h"

// ============Motor Controller============
#include "DRV8833_ChannelController.h"

DRV8833_ChannelController gMotorController(MOTOR_IN1, MOTOR_IN2, DRV8833_nSLP, DRV8833_nFAULT, &gLockAccelerometerObserver, -1);

#ifdef MOTOR_CONTROLLER_TEST
MotorControllerTest gMCT(&gMotorController);
#endif

// ============Lock System Controller============
#include "LockSystemController.h"

LockSystemController gLockSystemController(
		&gMotorController,
		&gLockAccelerometerObserver
		//change instantiation params as needed!
		);

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

//					SCHEDULING OBJECTS					//
Runnable * gSubjects[] = {
	&gAccelerometerSubject,
#ifdef DO_BATT
	&gBattSubj,
#endif
	NULL
};

Runnable * gControllers[] = {
	&gMotorController,
	&gLockSystemController,
#ifdef MOTOR_CONTROLLER_TEST
	&gMCT,
#endif
	NULL
};
//The arduino runs the setup function first
void setup() 
{ 
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(12,OUTPUT); // why?? pin is not connected in schematic
  pinMode(11,OUTPUT); // why?? pin is not connected in schematic

  debug_init();
  
	debugPrintln(F("init accel..."));
	//accel_init(); // should be done as early as possible to give it as much time to calibrate
	gAccelerometerSubject.init();
	debugPrintln(F("done"));
  
#ifdef HAVE_BLE
	debugPrintln(F("init ble..."));
	gBLE.init();
	debugPrintln(F("done"));
#endif
  
#ifdef MOTOR_CONTROLLER_TEST
	gMotorController.init(&gLockSystemController);
	gMCT.init();
#else
	debugPrintln(F("init motor..."));
	// motor_init();
	gMotorController.init(&gLockSystemController);
	/*
	debugPrintln(F("motor hard test"));
	//motorTime(500);
	//motorTime(-500);
	//gLockSystemController.cmdLock(); // default params - go to 180
	// will likely block / fault
	//gLockSystemController.cmdUnlock(); // default params - go to 0
	// will likely block / fault
	//gLockSystemController.cmdLock();
	//gLockSystemController.calibrateLockedPosition();
	//gLockSystemController.cmdUnlock();
	//gLockSystemController.calibrateUnlockedPosition();
	debugPrintln(F("done"));
	*/
#endif
	debugPrintln(F("setup done"));
}


void loop()
{
  // UPDATE
  delay(LOOP_TIME);
  loopCount++;
  sleepCount++;
	int angle_xy = gLockAccelerometerObserver.getLockAngleDeg(xy);
	int angle_xz = gLockAccelerometerObserver.getLockAngleDeg(xz);
	int angle_yz = gLockAccelerometerObserver.getLockAngleDeg(yz);
	Cartesian c;
	gLockAccelerometerObserver.getCart(&c);
  //update_motor();
  
  if (loopCount == loopsToUpdate){
    loopCount = 0;
    statusLED = !statusLED;
    digitalWrite(13,statusLED);
		debugPrint(F("Current Orientation: "));
		//debugPrint(angle_xy); debugPrint(SPACE);
		debugPrint(angle_xz); debugPrint(SPACE);
		debugPrintln(angle_yz);
		debugPrint(F("Current Acc: "));
		debugPrint(c.x); debugPrint(SPACE);
		debugPrint(c.y); debugPrint(SPACE);
		debugPrintln(c.z);
		// debugPrint(F("Awake for: ")); debugPrint(sleepCount); debugPrintln(F(" loops"));
#ifdef DO_BATTERY
		debugPrint(F("Battery: "));
		debugPrint(gBattObs.getCurVolts()); debugPrint(SPACE);
		debugPrint(gBattObs.getAvg100ms()); debugPrint(SPACE);
		debugPrint(gBattObs.getAvg1s()); debugPrint(SPACE);
		debugPrintln(gBattObs.getAvg10s());
#endif
  }

  if (sleepCount >= loopsToSleep){sleep();}

	// Sensors
	for (Runnable ** run = gSubjects; *run != NULL; run++) {
		(*run)->timeSlice();
	}

   // CONTROL
	for (Runnable ** run = gControllers; *run != NULL; run++) {
		(*run)->timeSlice();
	}
  
  String command; // read commands sent by user
#ifdef HAVE_BLE
	command=gBLE.readBLE();
	receivedCommand(command);
#endif
  command=readSerial();
#ifdef MOTOR_CONTROLLER_TEST
	gMCT.input(command.c_str());
#else
	receivedCommand(command);
#endif
}

void receivedCommand(String command){
  if (command.length() > 0){
		debugPrint(F("Got Command: "));debugPrintln(command);
    // execute the user commands
    executeCommandFromUser(command);
    debugCommandFromUser(command);
    resetSleepCounter();
  }
}

//**********************************************************************************************//
//					HELPER FUNCTIONS  				 	//
//**********************************************************************************************//
