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

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void BLE_init()
{
  BLE.begin(9600); // set baud rate for main communication port
}

void writeToBLE(int count) {
  BLE.println(count);
}
