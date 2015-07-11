/*

messages that might come from BLE113

response when setting mode: 0 2 6 1 12 2 0 0 5 1 0 0 5 3

eg. "occ"
111
99
99

*/

//**********************************************************************************************//
//          DEFINES             //
//**********************************************************************************************//

#define   BAUD_RATE          38400
#define         BLE_MODE_DELAY     200
#define         BLE_INPUT_DELAY    1

//**********************************************************************************************//
//          GLOBAL VARIABLES          //
//**********************************************************************************************//

SoftwareSerial BLE(3, 4); // RX, TX

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
  int i=0;
  BLE.begin(BAUD_RATE);
  delay(BLE_MODE_DELAY);    //wait to reset the ble
  
  Serial.println("Setup...");
  sendCommand(7,setup_cmd);
  Serial.println("Bond...");
  sendCommand(6,bond_cmd);
  Serial.println("Mitm...");
  sendCommand(8,mitm_cmd);  
}

String readBLE() {
  String outputString = "";
  while (BLE.available()){
    outputString += (char)getNextData();
  }
  if(outputString.length() > 0){
    Serial.print("Got String: ");Serial.println(outputString);
  }
  return outputString;
}

void sendCommand(int len, int command[]){
  int i;
  for(i=0; i<len; i++){
    BLE.write(command[i]);
    delay(BLE_INPUT_DELAY);
  }
  delay(BLE_MODE_DELAY);
}

int getNextData(){ // returns next byte in serial com
  int data = BLE.read();
  Serial.print("Got Byte: ");Serial.println(data);
  return data;
}
