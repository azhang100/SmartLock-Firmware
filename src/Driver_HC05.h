
//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define		BAUD_RATE          9600

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

SoftwareSerial BLE(3, 4); // RX, TX

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
  BLE.begin(BAUD_RATE);
  BLE.println("BLE online");
}

String readBLE(){
  String outputString = "";
  if (BLE.available()){
    while (BLE.available()) {
      char inChar = (char)BLE.read();
      outputString += inChar;
    }
  }
  else if (Serial.available()){
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      outputString += inChar;
    }
  }
  
  if(outputString.length() > 0)
  {BLE.print("Got String: "); BLE.println(outputString);}
  
  return outputString;
}
