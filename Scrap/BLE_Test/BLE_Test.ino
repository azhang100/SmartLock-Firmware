/*

messages that might come from BLE113

response when setting mode: 0 2 6 1 12 2 0 0 5 1 0 0 5 3
when recieved message over ble: 128 A 2 0 0 0 B 0 0 0 Y Z....Z

where A and B vary depending on how the message is received
Y is the length of Z...Z
Z...Z is the actual message, which is a series of bytes

*/

#include <SoftwareSerial.h>

int reset_evt[16]={0x80, 0x0C, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00 ,0x7A, 0x00, 0x03, 0x00, 0x01, 0x07};
int setup_evt[6]={0x00, 0x02, 0x06, 0x01, 0x00, 00};
int connect_evt[12]={0};

//int pswd_cmd[10]={ 0x09, 0x00, 0x05, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00};
int setup_cmd[7]={ 0x06, 0x00, 0x02, 0x06, 0x01, 0x02, 0x02 };
int bond_cmd[06]={0x05, 0x00, 0x01, 0x05, 0x01, 0x01};
int mitm_cmd[8]={0x07, 0x00, 0x03, 0x05, 0x03, 0x01, 0x07, 0x03};
int sending_cmd[2]= { 0x11, 0x15};

SoftwareSerial mySerial(3, 4); // RX, TX

int pin13 = HIGH;
int count = 0;

char flag=0;

void sendCommand(int len, int command[]){
  int i;
  for(i=0; i<len; i++){
    mySerial.write(command[i]);
    delay(2);
  }
  delay(1000);
}

void setup()  
{
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Goodnight moon!");
  
  mySerial.begin(38400); // hopefully we can pull this high frequency off
  
  Serial.println("Setup");
  sendCommand(7,setup_cmd);
  Serial.println("Bond");
  sendCommand(6,bond_cmd);
  Serial.println("Mitm");
  sendCommand(8,mitm_cmd);
  Serial.println("Ready");
}

void loop(){ // run over and over
  if (mySerial.available()){
    Serial.println(mySerial.read());
  }
}

/*

void loop(){ // run over and over
  if (mySerial.available()){
    String data = interpret();
    Serial.println(data);
  }
}

String interpret(){
  int i;
  String inputString = "";
  int eventType = mySerial.read();
  if (eventType == 128){ // data[0] means that it has an actual value
    for (i=0; i<9; i++){getNextData();} // burn values data[1:10]
    int lengthOfMessage = getNextData();
    
    char inChar;
    for (i=0; i<lengthOfMessage; i++){
      inChar = (char)mySerial.read(); 
      inputString += inChar;
    }
  }
  return inputString;
}
  
int getNextData(){
  int count = 0;
  while (count < 10){
    if (mySerial.available()){
      int data = mySerial.read();
      return(data);
    }
    else {
      count = count + 1;
      delay(3);
    }
  }
  return 0;
}

*/
