
//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define		BAUD_RATE          9600

//**********************************************************************************************//
//				   EXPOSED FUNCTIONS				 	//
//**********************************************************************************************//

void BLE_init();
String readBLE();               // returns text from BLE

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void BLE_init()
{
  BLEBegin(BAUD_RATE);
  BLEPrintln("BLE online");
}

String readBLE(){
  String outputString = "";
  if (BLEAvailable()){
    while (BLEAvailable()) {
      char inChar = (char)BLERead();
      outputString += inChar;
    }
  }
  if(outputString.length() > 0)
  {BLEPrint("Got String: "); BLEPrintln(outputString);}
  return outputString;
}
