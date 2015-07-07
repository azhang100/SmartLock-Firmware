#ifndef _LIS3DH_H
#define _LIS3DH_H
//*******************************************************************************************//
//                   			 INCLUDES         		                     //
//*******************************************************************************************//
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

//Include all the relevent Libraries on which this header file is dependent 
//eg include the library which contains the declaration for <BLE> module and its pins

//*******************************************************************************************//
//                   			 DEFINES         		                     //
//*******************************************************************************************//

#define        SLAVE_ADDR      0b0011000   //Slave address of Accelerometer//If SDO/SA0 is high '1' then the address changes to 0b0011001// If its low then the address is 0b0011000 
#define        ACCEL_REG_XL    0x28        //Accelerometer axes register which data will be stored
#define        ACCEL_REG_XH    0x29        //Needs to be changed which depends on Accelerometer
#define        ACCEL_REG_YL    0x2A
#define        ACCEL_REG_YH    0x2B
#define        ACCEL_REG_ZL    0x2C
#define        ACCEL_REG_ZH    0x2D
#define        MAX_ANGLE    	180        //Motor Max Rotation Angle
#define        MIN_ANGLE    	0        //Motor Min Rotation Angle
#define        ANGLE_INCRE    	1        //decides the incrementation between the 2 consecutive angles
#define        OPEN_ANGLE    	120        //Close angle for the door lock
#define        CLOSE_ANGLE    	50        //Open angle for the door lock
#define        PI               3.14
#define        ERR              .01

//#define        ANG_JUMP         140       //Angle Jump due to atan mapping//at its discontinuous points


//*******************************************************************************************//
//                   			FUNCTIONS DECLARATIONS                     	     //
//*******************************************************************************************//
void accel_init();            //Accelerometer initialization//Depends on the type of accelerometer
int getData(int reg_addr);    //returns data from 2 set of consecutive registers//Can be made for 1 register
float getAngle();                //Gives the angle rotation of motor
char getPosition();            //Gives the position whether [Closed:Open:Intermediate]
float filterData(float *curAngle, float *prevAngle, float *coVar);

//******************************************************************************************//
//                    			    FUNCTIONS		                            //
//******************************************************************************************//
/*
    Initialialization of TWI [Two wire Interface](I2C) for the Accelerometer so that the I2C pins
    gets activated.
*/
void accel_init(){
    Wire.begin();
        Wire.beginTransmission(SLAVE_ADDR);      //transmission with this address
       
       //Power Management Settings for MPU6050. which is not needed for LIS3DH//
       /*
        Wire.write(0x6B);          //first specifies power management address of MCU
        Wire.write(0);               //awakes MCU by sending 0 to above register address
        Wire.endTransmission(true);
        */
        
    /* Code to set some initial registers of accelerometer
       before starting the actual transmission */
    /* e.g setting the power management setting in MPU 6050
       and also selecting the desired range of accelerometer
       which is very essestial part as it determines the
       resolution of our angle */
       
}

/*     Function:    The main aim of this function is to give the data stored in particular registers
    Parameter:    Take the register address from which data has to extracted
       Retuns:        It returns the data stored in 2 consecutive addresses of accelerometer
*/
int getData(int reg_addr){
    unsigned int data=0;
    Wire.beginTransmission(SLAVE_ADDR);     //starting the communication again
      Wire.write(reg_addr);              //start with this register address (its the first data register
      Wire.endTransmission(false);          //continue to read data
      Wire.requestFrom(SLAVE_ADDR,2,true);      //request the slave to send the 2 byte data
      data=Wire.read()<<8|Wire.read();      //data is 16 bit data .the data is automatically read sequently

//Need to add extra loop and code for error codes returned by  Wire.endTransmission//
    Wire.endTransmission();
return data;
}

/*
    Function:     map(value, fromLow, fromHigh, toLow, toHigh)
    Description:    Re-maps a number from one range to another.That is,a value of fromLow would
            get mapped to toLow, a value of fromHigh to toHigh, values in-between to values
            in-between.
*/
float getAngle(){
  float curAngle;
  //curAngle=map(getData(ACCEL_REG_XL),-16384,16384,0,180);
  curAngle=(180*atan(((float)getData(ACCEL_REG_XL))/getData(ACCEL_REG_YL))/PI)+90;
  if(getData(ACCEL_REG_YL)>0) curAngle=180+curAngle;
  
  return curAngle;
  
}

/*
    Returns the the position of the (open, closed, unknown) of the knob
*/
char getPosition(){
  char pos = 'j';
  if (getAngle() <= CLOSE_ANGLE) {pos = 'c';}
  else if (getAngle() >= OPEN_ANGLE) {pos = 'o';}
  //if (angle < closedPot()) {pos = 'c';}
  //else if (angle > openPot()) {pos = 'o';}
  else {pos = 'm';}
  return pos;
} 

float filterData(float *curAngle, float *prevAngle, float *coVar){
  float kalGain;
  kalGain=*coVar/(*coVar+ERR);
  *coVar=(1-kalGain)*(*coVar);
  *prevAngle=*prevAngle+kalGain*(*curAngle-*prevAngle);
  return *prevAngle;
}


#endif
