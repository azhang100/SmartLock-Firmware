#include <Arduino.h>
#include <SoftwareSerial.h>
//==========Quick remapping===========//

SoftwareSerial BLE(3, 4); // RX, TX
// SoftwareSerial debug(3, 4);

void debugBegin(long i)     {Serial.begin(i);}
void debugEnd()             {Serial.end();}
void debugPrint(String s)   {Serial.print(s);}
void debugPrint(const char * s)   {Serial.print(s);}
void debugPrintInt(int i)      {Serial.print(i);}
void debugPrint(int i)      {Serial.print(i);}
void debugPrintln(String s) {Serial.println(s); Serial.flush();}
void debugPrintlnInt(int i) {Serial.println(i); Serial.flush();}
void debugPrintln(int i)    {Serial.println(i); Serial.flush();}
void debugWrite(byte b)     {Serial.write(b);}
int debugAvailable()        {return Serial.available();}
byte debugRead()            {return Serial.read();}

void BLEBegin(long i)       {BLE.begin(i);}
void BLEEnd()               {BLE.end();}
void BLEPrint(String s)     {BLE.print(s);}
void BLEPrintln(String s)   {BLE.println(s);}
void BLEWrite(byte b)       {BLE.write(b);}
int BLEAvailable()          {return BLE.available();}

byte BLERead()
{

	int data = BLE.read();
	debugPrint(F("Got Byte: "));debugPrintlnInt(data);
	return data;
}

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
