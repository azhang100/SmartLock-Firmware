#include <Arduino.h>
#include "Debug.h"

void debug_init()
{
	debugBegin(57600);
}

String readSerial()
{
	String outputString = "";
	if (debugAvailable()){
		while (debugAvailable()) {
			char inChar = (char)debugRead();
			outputString += inChar;
		}
	}
	if(outputString.length() > 0)
	{debugPrint(F("Got String: ")); debugPrintln(outputString);}
	return outputString;
}
