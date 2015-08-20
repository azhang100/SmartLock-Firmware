#include <SoftwareSerial.h>
#include "Debug.h"
#include "Driver_BLE113.h"

int BLE::reset_evt[16]={0x80, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00 ,0x7A, 0x00, 0x03, 0x00, 0x01, 0x07};
int BLE::setup_evt[6]={0x00, 0x02, 0x06, 0x01, 0x00, 00};
int BLE::connect_evt[12]={0};

//int pswd_cmd[10]={ 0x09, 0x00, 0x05, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
int BLE::setup_cmd[7]={ 0x06, 0x00, 0x02, 0x06, 0x01, 0x02, 0x02 };
int BLE::bond_cmd[6]={0x05, 0x00, 0x01, 0x05, 0x01, 0x01};
int BLE::mitm_cmd[8]={0x07, 0x00, 0x03, 0x05, 0x03, 0x01, 0x07, 0x03};
int BLE::sending_cmd[2]={0x11, 0x15};

BLE::BLE(int rx, int tx) :
	SoftwareSerial(rx, tx)
{
}

byte BLE::Read()
{

	int data = read();
	debugPrint(F("Got Byte: "));debugPrintlnInt(data);
	return data;
}

void BLE::init()
{
	int i=0;
	begin(BAUD_RATE);
	delay(BLE_MODE_DELAY);    //wait to reset the ble

	debugPrintln(F("Setup..."));
	sendCommand(7,setup_cmd);
	Read();
	debugPrintln(F("Bond..."));
	sendCommand(6,bond_cmd);
	Read();
	debugPrintln(F("Mitm..."));
	sendCommand(8,mitm_cmd);
	Read();
}

String BLE::readBLE() {
	String outputString = "";
	if (available()){
		int i;
		int eventType = Read();

		if (eventType == 128){ // data[0]==128 means that it has an actual value
			for (i=0; i<9; i++){Read();} // burn values data[1:10]
			int lengthOfMessage = Read();

			char inChar;
			for (i=0; i<lengthOfMessage; i++){
				inChar = (char)Read();
				outputString += inChar;
			}
			debugPrint(F("Got String: "));debugPrintln(outputString);
			//debugPrint(F("String Len: "));debugPrintln(outputString.length());
			//return outputString;
		}
		else {
			debugPrintln(F("Got Unknown Command"));
			//return outputString;
		}
	}
	if(outputString.length() > 0)
	{debugPrint(F("Got String: "));debugPrintln(outputString);}
	return outputString;
}

void BLE::sendCommand(int len, int command[]){
	int i;
	for(i=0; i<len; i++){
		write(command[i]);
		delay(BLE_INPUT_DELAY);
	}
	delay(BLE_MODE_DELAY);
}
