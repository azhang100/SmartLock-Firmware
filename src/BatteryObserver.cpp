#include "BatteryObserver.h"
#include "BatterySubject.h"
#include "Debug.h"


BatteryObserver::BatteryObserver( BatterySubject * s) :
	Observer(s)
{
}

void BatteryObserver::init()
{
	curVolts = avg100ms = avg1s = avg10s = ((BatterySubject *) mySubject)->getVoltage();

#ifdef NOT_USED
	samp10ms.fill(curVolts);
	samp100ms.fill(curVolts);
	samp1s.fill(curVolts);
#endif

	initDone = true;
}

void BatteryObserver::Update()
{
	if (!initDone) {
		init();
	}

	if (millis() - lastUpdate_ms < SAMP_TIME_MS) {
		return;
	}

	float oldVolts = curVolts;
	curVolts = ((BatterySubject *) mySubject)->getVoltage();
	slope10ms = (curVolts / oldVolts) - 1.0;

	update100ms(curVolts);

#ifdef NOT_USED
	samp10ms.addVal(curVolts);
	avg100ms = samp10ms.getAvg();

	samp100ms.addVal(avg100ms);
	avg1s = samp10ms.getAvg();

	samp1s.addVal(avg1s);
	avg10s = samp1s.getAvg();
#endif

	lastUpdate_ms = millis();
}

void BatteryObserver::update100ms(float smp)
{
	if (++ctr10ms > 9) {
		float old = avg100ms;
		avg100ms = avg100ms * SAMP_ALFA + (1-SAMP_ALFA) * smp;
		float diff = avg100ms - old;
		slope100ms = slope100ms * SAMP_ALFA + (1-SAMP_ALFA) * diff;
		update1s(avg100ms);
		ctr10ms = 0;
	}
}

void BatteryObserver::update1s(float smp)
{
	if (++ctr100ms > 9) {
		float old = avg1s;
		avg1s = avg1s * SAMP_ALFA + (1-SAMP_ALFA) * smp;
		float diff = avg1s - old;
		slope1s = slope1s * SAMP_ALFA + (1-SAMP_ALFA) * diff;
		update10s(avg1s);
		ctr10ms = 0;
	}
}

void BatteryObserver::update10s(float smp)
{
	if (++ctr1s > 9) {
		float old = avg10s;
		avg10s = avg10s * SAMP_ALFA + (1-SAMP_ALFA) * smp;
		float diff = avg10s - old;
		slope10s = slope10s * SAMP_ALFA + (1-SAMP_ALFA) * diff;
		ctr10ms = 0;
	}
}

float BatteryObserver::getCurVolts()
{
	return curVolts;
}

float BatteryObserver::getAvg100ms()
{
	return avg100ms;
}

float BatteryObserver::getAvg1s()
{
	return avg1s;
}

float BatteryObserver::getAvg10s()
{
	return avg10s;
}

float BatteryObserver::getSlope10ms()
{
	return slope10ms;
}

float BatteryObserver::getSlope100ms()
{
	return slope100ms;
}

float BatteryObserver::getSlope1s()
{
	return slope1s;
}

float BatteryObserver::getSlope10s()
{
	return slope10s;
}


#ifdef NOT_USED
void BatteryObserver::SampleData::fill(float f)
{
	for (uint8_t i = 0; i < BAT_SAMP_SIZE; i++) {
		arr[i] = f;
	}
}

void BatteryObserver::SampleData::addVal(float f)
{
	if (idx >= BAT_SAMP_SIZE) {
		idx = 0;
	}
	arr[idx++] = f;
}

float BatteryObserver::SampleData::getAvg()
{
	float avg = 0;
	for (uint8_t i = 0; i < BAT_SAMP_SIZE; i++) {
		avg += arr[i];
	}
	return avg / BAT_SAMP_SIZE;
}
#endif
