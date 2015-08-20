#include "STMACC_Subject.h"
#include <Arduino.h>
//#include "StringConsts.h"
#include "Debug.h"

STMACC_Subject::STMACC_Subject(ChipAddr ca) :
	chipAddr(ca), consFaultThresh(20)
{
	// to comply with arduino setup()/loop(), leave initialization interactions
	// to init() method
}

void STMACC_Subject::init() {
	conseqFaults = 0;
	Wire.begin();
	delay( 100 ); // 5 ms boot procedure 26 27 // reboot memory content, to make a clean start 28
	Wire.beginTransmission(chipAddr);
	Wire.write(CTRL_REG5_A);
	Wire.write( FIFO_EN );
	Wire.endTransmission();
	delay( 25 );
	Wire.beginTransmission(chipAddr);
	Wire.write(CTRL_REG1_A);
	Wire.write( ODR_NormalLow50Hz|Xen|Yen|Zen );
	Wire.endTransmission(); // set BDU= 1, scale = +/-2g, high resolution enabled 42
	delay( 25 );
	Wire.beginTransmission(chipAddr);
	Wire.write(CTRL_REG4_A);
	Wire.write( 0x80 );
	Wire.endTransmission();
	delay( 25 );
	debugPrintln(F("ACC init done"));
}

int STMACC_Subject::getData(int * accX, int * accY, int * accZ) {
	*accX = accelData.accX;
	*accY = accelData.accY;
	*accZ = accelData.accZ;
	if (overWrite) {
		return -1;
	}
	/* TBD
	   if (timeOut) {
	   return -2;
	   }
	   */
	return 0;
}

void STMACC_Subject::timeSlice() {
	if (sensorDataAvailable()) {
#ifdef STMACC_DEBUG
		debugPrintln(F("ACC new data"));
#endif
		if (readSensorData() == 0) {
			Notify();
		}
	}
}

int STMACC_Subject::sensorDataAvailable()
{
	Wire.beginTransmission(chipAddr);
	Wire.write(STATUS_REG_A);
	Wire.endTransmission();
	delay( 2 );
	Wire.requestFrom(chipAddr, 1 );
	if (Wire.available() >= 1 ){
		statusReg = Wire.read();
	}
	return (bitRead(statusReg, 3 ) == 1 );
}

int STMACC_Subject::readSensorData(){
	// read the result
	Wire.beginTransmission(chipAddr);
	Wire.write(OUT_X_L_A | 0x80 ); // read multiple bytes
	Wire.endTransmission();
	delay( 2 );
	Wire.requestFrom(chipAddr, 6 );
	overWrite = bitRead(statusReg, 7);
	int avail = Wire.available();
	if (overWrite || avail < 6) {
		if (overWrite) {
			totOverwrites++;
		}
		if (avail < 6) {
			++totFaults;
		}
		if (++conseqFaults > consFaultThresh) {
			Serial.print(F("ACC totF consF ovrwrt"));
			Serial.print(totFaults); Serial.print(SPACE);
			Serial.print(conseqFaults); Serial.print(SPACE);
			Serial.print(totOverwrites); Serial.print(SPACE);
			Serial.println(F(" resetting"));
			init();
		}
		return -1;
	}
	else {
		conseqFaults = 0;
	}

	for ( int i = 0 ; i < 6 ; i++ ){
		buffer[i] = Wire.read();
	}
	// calculation
	// this ignores the resolution - if we are in 12 bit mode,
	// the lowest 4 bits of every item will always be 0.
	// Optionally we can right shift 4 bits if we care.
	accelData.accX = (buffer[ 1 ] << 8 ) | buffer[ 0 ];
	accelData.accY = (buffer[ 3 ] << 8 ) | buffer[ 2 ];
	accelData.accZ = (buffer[ 5 ] << 8 ) | buffer[ 4 ];

#ifdef STMACC_DEBUG
	debugPrint(F("STMACC x y z overwrite:"));
	debugPrint(accelData.accX); debugPrint(SPACE);
	debugPrint(accelData.accY); debugPrint(SPACE);
	debugPrint(accelData.accZ); debugPrint(SPACE);
	debugPrintln(overWrite);
#endif
	delay( 2 ); // TBD - determine if this is needed - we only read when data avail,
	// and we won't run again till our next timeslice
	// if we want to ensure 2 ms between measurements, better to keep track
	// of last call time and return immediately if less than 2ms elapsed
	// so we don't hold up rest of system
	return 0;
}
