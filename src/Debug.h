#ifndef DEBUG_H
#define DEBUG_H

#define MOTOR_CONTROLLER_DEBUG
#undef ACCELEROMETER_OBSERVER_DEBUG
#undef STMACC_DEBUG

#define debugBegin(x)	Serial.begin(x)
#define debugEnd()	Serial.end()
#define debugPrint(x)	Serial.print(x)
#define debugPrintln(x)	Serial.println(x)
#define debugPrintInt(x)	Serial.print(x)
#define debugPrintlnInt(x)	Serial.println(x)
#define debugWrite(x)	Serial.write(x)
#define debugAvailable()	Serial.available()
#define debugRead()	Serial.read()


#include <Arduino.h>

void debug_init();
String readSerial();

#endif
