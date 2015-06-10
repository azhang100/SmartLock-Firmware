#ifndef _MPU6050_SIMPLELIB_H
#define _MPU6050_SIMPLELIB_H
//*******************************************************************************************//
//                   			 INCLUDES         		                     //
//*******************************************************************************************//

//*******************************************************************************************//
//                   			 DEFINES         		                     //
//*******************************************************************************************//

#define        SLAVE_ADDR      0x68        //Slave address of Accelerometer//For MPU6050 its 0x68
#define        ACCEL_REG_XL    0x3B        //Accelerometer axes register which data will be stored//For MPU6050 Its 0x3B
#define        ACCEL_REG_XH    0x3C        //Needs to be changed which depends on Accelerometer
#define        ACCEL_REG_YL    0x3D
#define        ACCEL_REG_YH    0x3E
#define        ACCEL_REG_ZL    0x3F
#define        ACCEL_REG_ZH    0x40
#define        ANGLE_INCRE    	1        //decides the incrementation between the 2 consecutive angles
#define        PI               3.14
#define        ACCEL_SLP        7          // bring up to turn on accelerator

//#define        ANG_JUMP         140       //Angle Jump due to atan mapping//at its discontinuous points


//*******************************************************************************************//
//                   			FUNCTIONS DECLARATIONS                     	     //
//*******************************************************************************************//
void accel_init();            //Accelerometer initialization//Depends on the type of accelerometer
int getData(int reg_addr);    //returns data from 2 set of consecutive registers//Can be made for 1 register
int getAngle();                //Gives the angle rotation of motor

//******************************************************************************************//
//                    			    FUNCTIONS		                            //
//******************************************************************************************//
/*
    Initialialization of TWI [Two wire Interface](I2C) for the Accelerometer so that the I2C pins
    gets activated.
*/
void accel_init(){
  
    pinMode(ACCEL_SLP,OUTPUT);
    digitalWrite(ACCEL_SLP,HIGH);
  
    Wire.begin();
        Wire.beginTransmission(SLAVE_ADDR);      //transmission with this address
        Wire.write(0x6B);          //first specifies power management address of MCU
        Wire.write(0);               //awakes MCU by sending 0 to above register address
        Wire.endTransmission(true);
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
int getAngle(){
  int curAngle;
  //curAngle=map(getData(ACCEL_REG_XL),-16384,16384,0,180);
  curAngle=(180*atan(((float)getData(ACCEL_REG_XL))/getData(ACCEL_REG_YL))/PI)+90;
  if(getData(ACCEL_REG_YL)>0) curAngle=180+curAngle;
  
  return curAngle;
  
}


#endif
