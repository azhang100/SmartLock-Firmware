#include "BatterySubject.h"
#include <Arduino.h>

BatterySubject::BatterySubject(unsigned char batPin) :
	myPin(batPin)
{
}

void BatterySubject::timeSlice()
{
	curVoltage = REFERENCE_VOLTAGE*analogRead(myPin)/1024.0;
	Notify();
}

float BatterySubject::getVoltage()
{
	return curVoltage;
}
