#ifndef BATTERY_OBSERVER_H
#define BATTERY_OBSERVER_H

#include "Observer.h"
#include <Arduino.h>

#ifdef NOT_USED
#define BAT_SAMP_SIZE 10
#endif

#define SAMP_TIME_MS 10
#define SAMP_ALFA 0.9

class BatterySubject;

class BatteryObserver : public Observer {
	public:
		BatteryObserver( BatterySubject * s );
		virtual void Update();
		float getCurVolts();
		/*
		float getAvg10ms();
		*/
		float getAvg100ms();
		float getAvg1s();
		float getAvg10s();
		float getSlope10ms();
		float getSlope100ms();
		float getSlope1s();
		float getSlope10s();
	protected:
		void init();
		void update100ms(float s);
		void update1s(float s);
		void update10s(float s);

#ifdef NOT_USED
		class SampleData {
			public:
				void addVal(float f);
				float getAvg();
				void fill(float f);
			private:
				float arr[BAT_SAMP_SIZE];
				uint8_t idx;
		};
#endif

		float curVolts, avg10ms, avg100ms, avg1s, avg10s;
		float curVoltDiff;
		float slope10ms, slope100ms, slope1s, slope10s;

#ifdef NOT_USED
		SampleData /* samp1ms, */ samp10ms, samp100ms, samp1s;
#endif

		unsigned long lastUpdate_ms;
		uint8_t ctr10ms, ctr100ms, ctr1s;
	private:
		bool initDone;
};

#endif
