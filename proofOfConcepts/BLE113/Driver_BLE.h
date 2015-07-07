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

#define		BAUD_RATE          38400
#define         BLE_MODE_DELAY     1000
#define         BLE_INPUT_DELAY    1

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

int reset_evt[16]={0x80, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00 ,0x7A, 0x00, 0x03, 0x00, 0x01, 0x07};
int setup_evt[6]={0x00, 0x02, 0x06, 0x01, 0x00, 00};
int connect_evt[12]={0};

//int pswd_cmd[10]={ 0x09, 0x00, 0x05, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
int setup_cmd[7]={ 0x06, 0x00, 0x02, 0x06, 0x01, 0x02, 0x02 };
int bond_cmd[06]={0x05, 0x00, 0x01, 0x05, 0x01, 0x01};
int mitm_cmd[8]={0x07, 0x00, 0x03, 0x05, 0x03, 0x01, 0x07, 0x03};
int sending_cmd[2]= { 0x11, 0x15};

int store_data[20];

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void BLE_init();
String readBLE();               // stores BLE text in inputstring
void sendCommand(int len, int command[]);
String interpret();
int getNextData();

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void BLE_init()
{
  int i=0;
  Serial.begin(BAUD_RATE);
  delay(BLE_MODE_DELAY);    //wait to reset the ble
  
  debug.println("Setup");
  sendCommand(7,setup_cmd);
  debug.println("Bond");
  sendCommand(6,bond_cmd);
  debug.println("Mitm");
  sendCommand(8,mitm_cmd);
  debug.println("Ready");
  
}

String readBLE() {
  String outputString = "";
    if (Serial.available()){
    
    int i;
    String outputString = "";
    int eventType = Serial.read();
    if (eventType == 128){ // data[0] means that it has an actual value
      for (i=0; i<9; i++){getNextData();} // burn values data[1:10]
      int lengthOfMessage = getNextData();
      
      char inChar;
      for (i=0; i<lengthOfMessage; i++){
        inChar = (char)Serial.read(); 
        outputString += inChar;
      }
    }
    debug.print("Got String: ");debug.println(outputString);
  }
  return outputString;
}

void sendCommand(int len, int command[]){
  int i;
  for(i=0; i<len; i++){
    Serial.write(command[i]);
    delay(BLE_INPUT_DELAY);
  }
  delay(BLE_MODE_DELAY);
}

String interpret(){
  int i;
  String inputString = "";
  int eventType = Serial.read();
  if (eventType == 128){ // data[0] means that it has an actual value
    for (i=0; i<9; i++){getNextData();} // burn values data[1:10]
    int lengthOfMessage = getNextData();
    
    char inChar;
    for (i=0; i<lengthOfMessage; i++){
      inChar = (char)getNextData(); 
      inputString += inChar;
    }
  }
  return inputString;
}
  
int getNextData(){ // returns next byte in serial com
  int count = 0;
  while (count < 30){
    if (Serial.available()){
      int data = Serial.read();
      debug.println(data);
      return(data);
    }
    
    count = count + 1;
    delay(BLE_INPUT_DELAY);
    
  }
  return 0;
}
