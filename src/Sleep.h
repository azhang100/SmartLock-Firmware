//*******************************************************************************************//
//                   			 DEFINES         		                     //
//*******************************************************************************************//

#define           DIGITAL_PIN_2_INT       0 // interrupt that corresponds with digital pin 2
#define           DIGITAL_PIN_3_INT       1 // interrupt that corresponds with digital pin 2

#define           WAKE_ACCEL              2 // pin used for waking up
#define           WAKE_BLE                3

#define           INT_ACCEL               DIGITAL_PIN_2_INT
#define           INT_BLE                 DIGITAL_PIN_3_INT

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//

//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void wakeUpNow();           // simply calling this function will wake it up.
void sleep_init();
void resetSleepCounter();
void sleep();               // sleeps the mpu
void sleepNow();            // this function the actual sleeping part of sleep() BUT DO NOT CALL IT DIRECTLY
void sleepStart();          // execute stuff right before you go to sleep
void sleepEnd();            // turn stuff back on

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

void wakeUpNow(){}       // simply calling this function will wake it up.

void sleep_init(){
  pinMode(WAKE_ACCEL, INPUT);
  pinMode(WAKE_BLE, INPUT);
}

void resetSleepCounter(){
  sleepCount = 0;
}

void sleepStart(){
  digitalWrite(13, LOW);
  //digitalWrite(MOTOR_B_SLP, LOW);
  gLockSystemController.sleep();
  BLEEnd();
  Serial.end();
}

void sleepEnd(){
  digitalWrite(13,HIGH);
  //digitalWrite(MOTOR_B_SLP, HIGH);
  gLockSystemController.wakeUp();
  BLEBegin(BAUD_RATE);
  Serial.begin(9600);
}

void sleep(){
  debugPrintln(F("Entering Sleep mode"));
  BLEPrintln(F("Entering Sleep mode"));
  delay(100);     // this delay is needed, the sleep 
                  //function will provoke a Serial error otherwise!!
  resetSleepCounter();
  
  sleepStart();
  sleepNow();
  sleepEnd();
  
  debugPrintln(F("Waking up"));
  BLEPrintln(F("Waking up"));
}

void sleepNow(){         // here we put the arduino to sleep DO NOT CALL THIS FUNCTION CALL SLEEP INSTEAD
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
                           // so sleep is possible. just a safety pin 

  attachInterrupt(INT_ACCEL, wakeUpNow, CHANGE); // use interrupt 0 (pin 2) and run function
                                     // wakeUpNow when pin 2 gets LOW 
  attachInterrupt(INT_BLE, wakeUpNow, LOW); // use interrupt 1 (pin 3) and run function
                                     // wakeUpNow when pin 3 gets LOW                                   

  sleep_mode();            // here the device is actually put to sleep!!
                           // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

  sleep_disable();         // first thing after waking from sleep:
                           // disable sleep...
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the 
                           // wakeUpNow code will not be executed 
                           // during normal running time.
}
