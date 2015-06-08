//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void BLE_init();
void readBLE();                // stores BLE text in inputstring

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void BLE_init()
{
  Serial.begin(BAUD_RATE);
}

void readBLE() {
  
  String inputString = "";
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
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
  Serial.println(count);
}
