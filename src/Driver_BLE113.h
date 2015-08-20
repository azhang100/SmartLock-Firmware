#ifndef DRIVER_BLE113_H
#define DRIVER_BLE113_H

#include "Config.h"
#include <Arduino.h>
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

class BLE : public SoftwareSerial {
	public:
		BLE(int rx, int tx);
		static int reset_evt[16];
		static int setup_evt[6];
		static int connect_evt[12];

//int pswd_cmd[10]={ 0x09, 0x00, 0x05, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
		static int setup_cmd[7];
		static int bond_cmd[6];
		static int mitm_cmd[8];
		static int sending_cmd[2];

		void init();
		byte Read();
String readBLE();               // returns text from BLE

	private:
		int store_data[20];

void sendCommand(int len, int command[]);

};
#endif
