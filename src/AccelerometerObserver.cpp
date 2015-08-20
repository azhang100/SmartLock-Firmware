//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : AccelerometerObserver.cpp
//  @ Date : 7/14/2015
//  @ Author : 
//
//

#include "AccelerometerObserver.h"
#include "AccelerometerSubject.h"
#include <Arduino.h>
//#include "StringConsts.h"
#include "Debug.h"

AccelerometerObserver::AccelerometerObserver( AccelerometerSubject * subject, float alpha) :
	Observer(subject), LPFalpha(alpha)
{
}

void AccelerometerObserver::Update() {

	AccelerometerSubject * subj = (AccelerometerSubject *) mySubject;
	int retVal = subj->getData(&rawAccel);

#ifdef ACCELEROMETER_OBSERVER_DEBUG
	debugPrint(F("AccelerometerObserver Update: x y z ret "));
	debugPrint(rawAccel.x); debugPrint(SPACE);
	debugPrint(rawAccel.y); debugPrint(SPACE);
	debugPrint(rawAccel.z); debugPrint(SPACE);
	debugPrint(retVal); debugPrint(SPACE);
	debugPrintln();
#endif
	// TBD - check retVal, decide what to do on overwrite or timeout, etc.
	//
	updateCart();
	//updateEuler();
	//call internal updateCart() to save the raw and filtered data
	//call internal updateEuler() to convert to angles and save raw and filtered
}

void AccelerometerObserver::getCart(Cartesian * cart, int raw)
{
	if (raw) {
		*cart = rawAccel;
	} else {
		*cart = filtAccel;
	}
}

/*
   void AccelerometerObserver::getEuler(EulerAngles * euler){
 *euler = filtEuler;
 }
 */

void AccelerometerObserver::updateCart() {
	// we have new raw data, update filtered values
	filtAccel.x = filtAccel.x * (1-LPFalpha) + rawAccel.x * LPFalpha;
	filtAccel.y = filtAccel.y * (1-LPFalpha) + rawAccel.y * LPFalpha;
	filtAccel.z = filtAccel.z * (1-LPFalpha) + rawAccel.z * LPFalpha;
}

/*
   void AccelerometerObserver::updateEuler() {
// save raw data, perform filtering, save filtered data

}
*/
