/*

messages that might come from BLE113

response when setting mode: 0 2 6 1 12 2 0 0 5 1 0 0 5 3
when recieved message over ble: 128 A 2 0 0 0 B 0 0 0 Y Z....Z

where A and B vary depending on how the message is received
Y is the length of Z...Z
Z...Z is the actual message, which is a series of bytes

*/

//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

#define	      	BAUD_RATE          38400
#define         BLE_MODE_DELAY     500
#define         BLE_INPUT_DELAY    1

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

int reset_evt[16]={0x80, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00 ,0x7A, 0x00, 0x03, 0x00, 0x01, 0x07};
int setup_evt[6]={0x00, 0x02, 0x06, 0x01, 0x00, 00};
int connect_evt[12]={0};

//int pswd_cmd[10]={ 0x09, 0x00, 0x05, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
int setup_cmd[7]={ 0x06, 0x00, 0x02, 0x06, 0x01, 0x02, 0x02 };
int bond_cmd[6]={0x05, 0x00, 0x01, 0x05, 0x01, 0x01};
int mitm_cmd[8]={0x07, 0x00, 0x03, 0x05, 0x03, 0x01, 0x07, 0x03};
int sending_cmd[2]={0x11, 0x15};

int store_data[20];

//**********************************************************************************************//
//				   EXPOSED FUNCTIONS				 	//
//**********************************************************************************************//

void BLE_init();
String readBLE();               // returns text from BLE

//**********************************************************************************************//
//				   NON-EXPOSED DECLARATIONS				 	//
//**********************************************************************************************//

void sendCommand(int len, int command[]);

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void BLE_init()
{
  int i=0;
  BLEBegin(BAUD_RATE);
  delay(BLE_MODE_DELAY);    //wait to reset the ble
  
  debugPrintln("Setup...");
  sendCommand(7,setup_cmd);
  BLERead();
  debugPrintln("Bond...");
  sendCommand(6,bond_cmd);
  BLERead();
  debugPrintln("Mitm...");
  sendCommand(8,mitm_cmd);  
  BLERead();
}

String readBLE() {
  String outputString = "";
  if (BLEAvailable()){
    int i;
    int eventType = BLERead();
    
    if (eventType == 128){ // data[0]==128 means that it has an actual value
      for (i=0; i<9; i++){BLERead();} // burn values data[1:10]
      int lengthOfMessage = BLERead();
      
      char inChar;
      for (i=0; i<lengthOfMessage; i++){
        inChar = (char)BLERead(); 
        outputString += inChar;
      }
    debugPrint("Got String: ");debugPrintln(outputString);  
    //debugPrint("String Len: ");debugPrintln(outputString.length());
      //return outputString;
    }
    else {
      debugPrintln("Got Unknown Command");
      //return outputString;
    }
  }
  if(outputString.length() > 0)
  {debugPrint("Got String: ");debugPrintln(outputString);}
  return outputString;
}

void sendCommand(int len, int command[]){
  int i;
  for(i=0; i<len; i++){
    BLEWrite(command[i]);
    delay(BLE_INPUT_DELAY);
  }
  delay(BLE_MODE_DELAY);
}
