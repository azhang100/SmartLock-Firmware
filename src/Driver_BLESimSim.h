
//**********************************************************************************************//
//          DEFINES             //
//**********************************************************************************************//

#define         BAUD_RATE          9600
#define         BLE_MODE_DELAY     200
#define         BLE_INPUT_DELAY    1

//**********************************************************************************************//
//           EXPOSED FUNCTIONS          //
//**********************************************************************************************//

void BLE_init();
String readBLE();               // returns text from BLE

//**********************************************************************************************//
//           NON-EXPOSED DECLARATIONS         //
//**********************************************************************************************//

void sendCommand(int len, int command[]);
int getNextData();

//**********************************************************************************************//
//          FUNCTIONS           //
//**********************************************************************************************//

void BLE_init()
{
  BLEBegin(BAUD_RATE);
  BLEPrintln("BLE online");
  debugPrintln("BLE online");
}

String readBLE() {
  String outputString = "";
  while (BLEAvailable()){
    outputString += (char)BLERead();
  }
  if(outputString.length() > 0){
    debugPrint("Got String: ");debugPrintln(outputString);
  }
  return outputString;
}
