#include "AccelerometerSubject.h"
#include <Arduino.h>
#include "Debug.h"
#include "Cartesian.h"
//#include "StringConsts.h"

int AccelerometerSubject::getData(int * x, int * y, int * z)
{
	*x = accelData.accX;
	*y = accelData.accY;
	*z = accelData.accZ;
#ifdef ACCELEROMETER_SUBJECT_DEBUG
	debugPrint(F("AccelerometerSubject getData: x y z "));
	debugPrint(accelData.accX); debugPrint(SPACE);
	debugPrint(accelData.accY); debugPrint(SPACE);
	debugPrintln(accelData.accZ);
#endif
	return 0;
}

int AccelerometerSubject::getData(Cartesian * c)
{
	c->x = accelData.accX;
	c->y = accelData.accY;
	c->z = accelData.accZ;
#ifdef ACCELEROMETER_SUBJECT_DEBUG
	debugPrint(F("AccelerometerSubject getData: x y z "));
	debugPrint(accelData.accX); debugPrint(SPACE);
	debugPrint(accelData.accY); debugPrint(SPACE);
	debugPrintln(accelData.accZ);
#endif
	return 0;
}
