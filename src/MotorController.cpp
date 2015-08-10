//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : MotorController.cpp
//  @ Date : 7/14/2015
//  @ Author : 
//
//


#include "MotorController.h"
#include "LockSystemController.h"
//#include "StringConsts.h"
#include <Arduino.h>
#include "Debug.h"

#ifdef MOTOR_CONTROLLER_DEBUG
const PROGMEM char * const MotorController::SPACE = " ";
#endif

MotorController::MotorController(LockAccelerometerObserver * observer,
		unsigned int minPower, int dirSign) :
	state(initial), driveMode(Idle), direction(CW),
	targetAngle(0), targetTime_ms(0), directionSign(dirSign),
	desiredPower(0), maxEngageTime_ms(500), myObserver(observer),
	lastTimeSliceAngle(myObserver->getLockAngleDeg()), minimumProgress(10),
	minPower(minPower), positionThreshhold(2)
{
}

void MotorController::init(LockSystemController * lsc) {
	myLSC = lsc;
}

void MotorController::timeSlice() {
#ifdef MOTOR_CONTROLLER_DEBUG
	static int counter = 0;
	if (++counter % 100 == 1) {
		stateInfo();
	}
#endif
	if (checkMotorFault()) {
		totalFaults++;
		contiguousFaults++;
	}
	else {
		contiguousFaults = 0;
	}

#ifdef MOTOR_CONTROLLER_DEBUG
	if (counter % 100 == 1) {
		debugPrint(F("MC:timeSlice tF, cF "));
		debugPrint(totalFaults); debugPrint(SPACE);
		debugPrintln(contiguousFaults);
	}
#endif

	switch (state)
	{
		case initial:
			{
				return;
			}
			break;
		case engaging:
			{
				int currentAngle = myObserver->getLockAngleDeg();
				int progress = currentAngle - lastTimeSliceAngle;
				lastTimeSliceAngle = currentAngle;
				if (abs(progress) > minimumProgress) {
					newState(driving);
					return;
				}
				int elapsed = millis() - driveStartTime_ms;
				if (elapsed > maxEngageTime_ms)
				{
					cmdCoast();
					newState(blocked);
					myLSC->motorStuck();
				}
			}
			break;
		case driving:
			{
				int elapsed = millis() - driveStartTime_ms;
				int currentAngle = myObserver->getLockAngleDeg();
				int progress = currentAngle - lastTimeSliceAngle;
				lastTimeSliceAngle = currentAngle;

				switch (driveMode) {
					case ForDuration:
						{
							if (elapsed >= targetTime_ms)
							{
								cmdStop(); // changes state
								myLSC->motorComplete();
								return;
							}
						}
						break;
					case ToPosition:
						{
							if (abs(currentAngle - targetAngle) < positionThreshhold)
							{
								cmdStop(); // changes state
								myLSC->motorComplete();
								return;
							}
						}
						break;
				}
				if (abs(progress) < minimumProgress && desiredPower != 0)
				{
					cmdCoast();
					newState(blocked);
					myLSC->motorStuck();
				}
			}
			break;
		case coasting:
			{
			}
			break;
		case stopped:
			{
			}
			break;
		case blocked:
			{
			}
			break;
	}
}

void MotorController::cmdStop() {
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrintln(F("MC:cmdStop"));
#endif
	// WHICH STATE IS THIS VALID FOR?
	if (state != stopped)
	{
		applyMotorStopCommands();
		driveMode = Idle;
		desiredPower = 0;
		newState(stopped);
	}
	return;
}

void MotorController::cmdCoast() {
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrintln(F("MC:cmdCoast"));
#endif
	if (state != coasting) {

		applyMotorCoastCommands();
		driveMode = Idle;
		newState(coasting);
	}
	return;
}

void MotorController::cmdDriveMotorToPosition(Direction direction, int power, int targetPos) {
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrint(F("MC:cmdDTP power target ")); debugPrint(power); debugPrint(SPACE); debugPrintln(targetPos);
#endif
	if (power < minPower) {
		return;
	}
	switch (state)
	{
		case initial:
		case stopped:
		case coasting:
		case engaging:
		case driving:
			{
				if ((state == driving) && (this->direction == direction))
				{
					newState(driving);
				}
				else {
					newState(engaging);
				}
				lastTimeSliceAngle = myObserver->getLockAngleDeg();
				targetAngle = targetPos;
				driveMode = ToPosition;
				this->direction = direction;
				driveStartTime_ms = millis();
				desiredPower = power;
				applyMotorDriveCommands();
				return;
			}
			break;
		case blocked:
			{
				return;
			}
			break;
	}
}

void MotorController::cmdDriveMotorForDuration(Direction direction, int power, unsigned long targetMs) {
	if (power < minPower) {
#ifdef MOTOR_CONTROLLER_DEBUG
		debugPrintln(F("MC:cmdDFD power too low"));
#endif
		return;
	}
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrint(F("MC:cmdDFD power target ")); debugPrint(power); debugPrint(SPACE); debugPrintln(targetMs);
	stateInfo();
#endif
	lastTimeSliceAngle = myObserver->getLockAngleDeg();
	switch (state)
	{
		case initial:
		case stopped:
		case coasting:
		case engaging:
			{
				lastTimeSliceAngle = myObserver->getLockAngleDeg();
				targetTime_ms = targetMs;
				driveMode = ForDuration;
				this->direction = direction;
				driveStartTime_ms = millis();
				desiredPower = power;
				applyMotorDriveCommands();
#ifdef MOTOR_CONTROLLER_DEBUG
				debugPrint(F("MC:cmdDFD start ")); debugPrintln(driveStartTime_ms);
#endif
				newState(engaging);
				return;
			}
			break;
		case driving:
			{
				lastTimeSliceAngle = myObserver->getLockAngleDeg();
				targetTime_ms = targetMs;
				driveMode = ForDuration;
				this->direction = direction;
				driveStartTime_ms = millis();
				desiredPower = power;
				applyMotorDriveCommands();
#ifdef MOTOR_CONTROLLER_DEBUG
				debugPrint(F("MC:cmdDFD start ")); debugPrintln(driveStartTime_ms);
#endif
				if (this->direction != direction)
				{
					newState(engaging);
				}
				return;
			}
			break;
		case blocked:
			{
				return;
			}
			break;
	}
}

void MotorController::sleep()
{
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrintln(F("MC:sleep"));
#endif
	applyMotorSleepCommands();
}

void MotorController::wakeUp()
{
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrintln(F("MC:wakeUp"));
#endif
	applyMotorWakeUpCommands();
}

void MotorController::newState(MotorController::State ns)
{
#ifdef MOTOR_CONTROLLER_DEBUG
	stateInfo();
#endif
	state = ns;
#ifdef MOTOR_CONTROLLER_DEBUG
	stateInfo();
#endif
}

void MotorController::stateInfo()
{
#ifdef MOTOR_CONTROLLER_DEBUG
	debugPrint(F("MC:stateInfo State, Direction, DrivingMode "));
	switch (state) {
		case initial:
			debugPrint(F("initial"));
			break;
		case driving:
			debugPrint(F("driving"));
			break;
		case blocked:
			debugPrint(F("blocked"));
			break;
		case stopped:
			debugPrint(F("stopped"));
			break;
		case engaging:
			debugPrint(F("engaging"));
			break;
		case coasting:
			debugPrint(F("coasting"));
			break;
	}
	debugPrint(direction == CW ? F(" CW ") : F(" CCW "));
	switch(driveMode) {
		case Idle: debugPrintln(F("Idle")); break;
		case ToPosition: debugPrintln(F("ToPosition")); break;
		case ForDuration: debugPrintln(F("ForDuration")); break;
	}
#endif
}

#ifdef MOTOR_CONTROLLER_TEST

const PROGMEM char * const MotorControllerTest::SPACE = " ";
const PROGMEM char * const MotorControllerTest::PLEASE = "Please";
const PROGMEM char * const MotorControllerTest::ENTER = "enter";
const PROGMEM char * const MotorControllerTest::SCENARIO = "scenario";
const PROGMEM char * const MotorControllerTest::DIRECTION = "direction";
const PROGMEM char * const MotorControllerTest::POWER = "power";
const PROGMEM char * const MotorControllerTest::DUR = "duration";
const PROGMEM char * const MotorControllerTest::OR = "or";
const PROGMEM char * const MotorControllerTest::CW = "CW";
const PROGMEM char * const MotorControllerTest::CCW = "CCW";
const PROGMEM char * const MotorControllerTest::FAILURE = "FAILURE";
const PROGMEM char * const MotorControllerTest::SUCCESS = "SUCCESS";

MotorControllerTest::MotorControllerTest(MotorController * mc) :
	mc(mc), scen(idle), s_DFD(initial), exitCode(-1)
{
}

void MotorControllerTest::p(int i, bool doNL)
{
	if (doNL) {
		Serial.println(i);
	}
	else {
		Serial.print(i);
	}
}
void MotorControllerTest::p(const char * msg, bool doNL)
{
	if (doNL) {
		Serial.println(msg);
	}
	else {
		Serial.print(msg);
	}
}

void MotorControllerTest::p(const __FlashStringHelper* msg, bool doNL)
{
	if (doNL) {
		Serial.println(msg);
	}
	else {
		Serial.print(msg);
	}
}

void MotorControllerTest::pp(const char * msgs[], bool doNL)
{
	const char * msg;
	while (msg = *msgs++) {
		Serial.print(msg);
	}
	if (doNL) {
		Serial.println("");
	}
}

void MotorControllerTest::timeSlice()
{
	switch (scen) {
		case idle:
			if (millis() - scenEntryTime_ms > msToPrompt) {
				pp(p_selScen, true);
				newScen(idle);
			}
			return;
			break;
		case DFD:
			timeSlice_DFD();
			break;
		case DRV:
			timeSlice_DRV();
			break;
	}
}

void MotorControllerTest::timeSlice_DRV()
{
	unsigned long elapsed = millis() - scenEntryTime_ms;
	unsigned short mode = (elapsed / 5000) % 5;
	static unsigned short lastMode = 999;
	if (mode != lastMode) {
		p(F("tsDRV scenEntry ")); p(scenEntryTime_ms); p(F(" elapsed ")); p(elapsed); p(F(" mode ")); p(mode); p(F(" lastMode ")); p(lastMode, true);
		switch (mode) {
			case 0:
				p(F("drive "));
				mc->direction = MotorController::CW;
				mc->desiredPower = 150;
				p(CW, true);
				mc->applyMotorDriveCommands();
				break;
			case 1:
				p(F("stop"), true);
				mc->applyMotorStopCommands();
				break;
			case 2:
				p(F("drive "));
				mc->direction = MotorController::CCW;
				mc->desiredPower = 150;
				p(CCW, true);
				mc->applyMotorDriveCommands();
				break;
			case 3:
				p(F("coast"), true);
				mc->applyMotorCoastCommands();
				break;
			case 4:
				p(F("stop"), true);
				mc->applyMotorStopCommands();
				break;
		}
	}
	lastMode = mode;
}
void MotorControllerTest::timeSlice_DFD()
{
	unsigned long timeSliceStart_ms = millis();
	switch (s_DFD) {
		case initial:
			pp(p_selDir, true);
			newState_DFD(waitForDir);
			break;
		case waitForDir:
			if (timeSliceStart_ms - stateEntryTime_ms > msToPrompt) {
				pp(p_selDir, true);
				newState_DFD(waitForDir);
			}
			break;
		case waitForPower:
			if (timeSliceStart_ms - stateEntryTime_ms > msToPrompt) {
				pp(p_selPower, true);
				newState_DFD(waitForPower);
			}
			break;
		case waitForDuration:
			if (timeSliceStart_ms - stateEntryTime_ms > msToPrompt) {
				pp(p_selDur, true);
				newState_DFD(waitForDuration);
			}
			break;
		case checkDriving:
			if (mc->state == MotorController::blocked)
			{
				exitCode = 2;
				newState_DFD(failure);
			}
			if ((timeSliceStart_ms - stateEntryTime_ms) < (duration - 10))
			{
				if (mc->state != MotorController::driving)
				{
					exitCode = 3;
					newState_DFD(failure);
				}
			}
			else if (((timeSliceStart_ms - stateEntryTime_ms) >= duration - 10) && ((timeSliceStart_ms - stateEntryTime_ms) <= duration + 10))
			{
				return;
			}
			else
			{
				newState_DFD(checkComplete);
			}
			break;
		case checkComplete:
			if (mc->state != MotorController::stopped)
			{
				exitCode = 4;
				newState_DFD(failure);
			}
			else
			{
				exitCode = 0;
				newState_DFD(success);
			}
			break;
		case success:
			newState_DFD(initial);
			p(SUCCESS);
			p(exitCode, true);
			init();
			break;
		case failure:
			p(FAILURE);
			p(exitCode, true);
			newState_DFD(initial);
			init();
			break;
	}
}


void MotorControllerTest::newState_DFD(StateDFD ns)
{
	stateEntryTime_ms = millis();
	s_DFD = ns;
}

void MotorControllerTest::newScen(Scenario s)
{
	scenEntryTime_ms = millis();
	scen = s;
}

void MotorControllerTest::input(const char * in)
{
	if (in && strlen(in)) {
		int choice = atoi(in);
		switch (scen) {
			case idle:
				if (choice > idle && choice < END) {
					newScen((MotorControllerTest::Scenario)choice);
				}
				break;
			case DFD:
				input_DFD(in);
				break;
			case DRV:
				input_DRV(in);
				break;
		}
	}
}

void MotorControllerTest::input_DFD(const char * in)
{
	int inval;
	switch (s_DFD) {
		case waitForDir:
			if (strcmp(in, CW) == 0) {
				dir = MotorController::CW;
				pp(p_selPower, true);
				newState_DFD(waitForPower);
			}
			else if (strcmp(in, CCW) == 0) {
				dir = MotorController::CCW;
				pp(p_selPower, true);
				newState_DFD(waitForPower);
			}
			else {
				pp(p_selDir, true);
			}
			break;
		case waitForPower:
			inval =  atoi(in);
			if (inval > 0 && inval < 256) {
				power = inval;
				pp(p_selDur, true);
				newState_DFD(waitForDuration);
			}
			else {
				pp(p_selPower, true);
			}
			break;
		case waitForDuration:
			inval =  atoi(in);
			if (inval > 0 && inval < 32000) {
				duration = inval;
				mc->minimumProgress = 0;
				mc->cmdDriveMotorForDuration(dir, power, duration);
				if (mc->state != MotorController::engaging)
				{
					exitCode = 1;
					newState_DFD(failure);
				}
				else {
					mc->state = MotorController::driving;
					newState_DFD(checkDriving);
				}
			}
			else {
				pp(p_selDur, true);
			}
			break;
	}
}

void MotorControllerTest::input_DRV(const char * in)
{
	if (strcmp(in, "s") == 0) {
		init();
	}
}

void MotorControllerTest::init()
{
	exitCode = -1;
	newScen(idle);
	pp(p_selScen , true);
}


#endif
