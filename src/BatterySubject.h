#ifndef BATTERY_SUBJECT_H
#define BATTERY_SUBJECT_H

#define REFERENCE_VOLTAGE 6.6 //Double of 3.3 since the voltage is halved before sensing

#include "Subject.h"
#include "Battery_config.h"

class BatterySubject : public Subject
{
	public:
		BatterySubject(unsigned char batPin = BATTERY_PIN);

		void timeSlice();
		float getVoltage(); // need virtual? not yet ...

	protected:
		unsigned char myPin;
		float curVoltage;
};
#endif
