#include <Servo.h> 
#include <SoftwareSerial.h>

int bluetoothRx = 3;
int bluetoothTx = 4;  

SoftwareSerial bluetooth(bluetoothRx, bluetoothTx);

Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
int potPin = A0;
int servoPin = 9;

int closedPos = 15;
int openPos = 170;
int pos = closedPos;    // variable to store the servo position

int closedPot = 450;
int openPot = 900;
int pot = 'c'; // on potentiometer

void setup() 
{ 
   Serial.begin(9600);
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object 
  bluetooth.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(8,HIGH);
} 
 
void loop() {
delay(100); // otherwise it's going to break
digitalWrite(13,HIGH);
delay(100);
digitalWrite(13,LOW);
 
 //==================Take commands================
 
 Serial.println("hey");
 bluetooth.println("hi");
 if (bluetooth.available() > 0) { 
   
   //char command=bluetooth.read();
 Serial.println("bluetooth available");
 //Serial.println(bluetooth.read());
// if (command=='o') {pos = openPos;}
// if (command=='c') {pos = closedPos;}
// myservo.write(pos);
 
  myservo.write(bluetooth.read());
 }

 //==================Status check================

//int sensorValue = analogRead(potPin);
// if (sensorValue < closedPot) {pot = 'c';}
// if (sensorValue > openPot) {pot = 'o';}
//Serial.println(sensorValue);
//Serial.println(pot);
}
