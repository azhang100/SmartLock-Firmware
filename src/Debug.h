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

  SoftwareSerial BLE(4, 3); // RX, TX
  // SoftwareSerial debug(3, 4);
  
  void debugBegin(long i)     {Serial.begin(i);}
  void debugEnd()             {Serial.end();}
  void debugPrint(String s)   {Serial.print(s);}
  void debugPrintln(String s) {Serial.println(s);}
  void debugPrintlnInt(int i) {Serial.println(i);}
  void debugWrite(byte b)     {Serial.write(b);}
  int debugAvailable()        {return Serial.available();}
  byte debugRead()            {return Serial.read();}
  
  void BLEBegin(long i)       {BLE.begin(i);}
  void BLEEnd()               {BLE.end();}
  void BLEPrint(String s)     {BLE.print(s);}
  void BLEPrintln(String s)   {BLE.println(s);}
  void BLEWrite(byte b)       {BLE.write(b);}
  int BLEAvailable()          {return BLE.available();}
  byte BLERead()              {

    int data = BLE.read();
    debugPrint("Got Byte: ");debugPrintlnInt(data);
    return data;
    }

//==========Actual functions===========//

void debug_init(){
  debugBegin(38400);
}

String readSerial(){
  String outputString = "";
  if (debugAvailable()){
    while (debugAvailable()) {
      char inChar = (char)debugRead();
      outputString += inChar;
    }
  }
  if(outputString.length() > 0)
  {debugPrint("Got String: "); debugPrintln(outputString);}
  return outputString;
}
