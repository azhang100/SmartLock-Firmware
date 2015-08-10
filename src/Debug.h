#ifndef DEBUG_H
#define DEBUG_H
/*

//==========Quick remapping===========//

// SoftwareSerial BLE(3, 4); // RX, TX
SoftwareSerial debug(3, 4);

void debugBegin(long i)     {debug.begin(i);}
void debugEnd()             {debug.end();}
void debugPrint(String s)   {debug.print(s);}
void debugPrintln(String s) {debug.println(s);}
void debugPrintlnInt(int i) {debug.println(i);}
void debugWrite(byte b)     {debug.write(b);}
int debugAvailable()        {return debug.available();}
byte debugRead()            {return debug.read();}

void BLEBegin(long i)       {Serial.begin(i);}
void BLEEnd()               {Serial.end();}
void BLEPrint(String s)     {Serial.print(s);}
void BLEPrintln(String s)   {Serial.println(s);}
void BLEWrite(byte b)       {Serial.write(b);}
int BLEAvailable()          {return Serial.available();}
byte BLERead()              {return Serial.read();}
 */

//==========Quick remapping===========//

void debugBegin(long i);
void debugEnd();
void debugPrint(String s);
void debugPrintInt(int i);
void debugPrint(int i);
void debugPrintln(String s);
void debugPrint(const char * s);
void debugPrintlnInt(int i);
void debugPrintln(int i);
void debugWrite(byte b);
int debugAvailable();
byte debugRead();

void BLEBegin(long i);
void BLEEnd();
void BLEPrint(String s);
void BLEPrintln(String s);
void BLEWrite(byte b);
int BLEAvailable();
byte BLERead();
void debug_init();
String readSerial();
#endif
