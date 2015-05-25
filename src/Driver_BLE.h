//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define		ARDUINO_RX_BLE_TX          3               //make pin 3 of arduino as RX and map it to TX of BLE
#define		ARDUINO_TX_BLE_RX          4               //make pin 4 of arduino as TX and map it to RX of BLE


//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

SoftwareSerial BLE = SoftwareSerial(ARDUINO_RX_BLE_TX, ARDUINO_TX_BLE_RX); // RX, TX of arduino 

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void BLE_init();                        //BLE initialization
void readBLE();

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void BLE_init()
{
  BLE.begin(BAUD_RATE); // set baud rate for main communication port
}

void readBLE() {
  
  String inputString = "";
  while (BLE.available()) {
    // get the new byte:
    char inChar = (char)BLE.read(); 
    //Serial.write(inChar);
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a semi colon, set a flag
    // so the main loop can do something about it:
    if (inChar == ';') {
//      int pos = inputString.indexOf(';');
//      inputString[pos+1] = '\n';
      commandAvailable = true;
      Serial.println(inputString);
      storeCommand(inputString);
    } 
  }
  
}

void writeToBLE(int count) {
  BLE.println(count);
}
