#include <Wire.h>


const int LIS3DH_ADDR = 0x18;
int pin = 13;
int int1Pin = 2;
int int2Pin = 3;


/* I2C Communication Set-up ---------------------------------------------------------*/
unsigned int readRegister(byte reg) {
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(LIS3DH_ADDR, 1);
  return Wire.read();
}


void writeRegister(byte reg, byte data) {
  Wire.beginTransmission(LIS3DH_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}


/* Initialize accelerometer in the host processor. ----------------------------------*/
//It only needs to be executed one time after power up in initialization routine.
void init_ACC(void)
{
    // configurations for control registers
    writeRegister(0x20, 0xA7); //Write A7h into CTRL_REG1;      // Turn on the sensor with ODR = 100Hz normal mode.
    writeRegister(0x21, 0x00); //Write 00h into CTRL_REG2;      // High-pass filter (HPF) disabled 
    writeRegister(0x22, 0x40); //Write 40h into CTRL_REG3;      // ACC AOI1 interrupt signal is routed to INT1 pin.
    writeRegister(0x23, 0x00); //Write 00h into CTRL_REG4;      // Full Scale = +/-2 g
    writeRegister(0x24, 0x00); //Write 00h into CTRL_REG5;      // Default value. Interrupt signals on INT1 pin is not latched. 
                                                                //Users don’t need to read the INT1_SRC register to clear the interrupt signal.
    // configurations for wakeup and motionless detection
    writeRegister(0x32, 0x10); //Write 10h into INT1_THS;          // Threshold (THS) = 16LSBs * 15.625mg/LSB = 250mg.
    writeRegister(0x33, 0x00); //Write 33h into INT1_DURATION;     // Duration = 1LSBs * (1/10Hz) = 0.1s.
    writeRegister(0x30, 0x0A); //Write 0Ah into INT1_CFG;          // Enable XLIE, YLIE interrupt generation, OR logic.
 
}
void setup() {
  delay(100);
  Wire.begin();
  Serial.begin(9600);
  init_ACC();
  
  int res = readRegister(0x0F); //WHO_AM_I
  Serial.println(res);
  pinMode(pin, OUTPUT);      // sets the digital pin 13 as output
  pinMode(int1Pin, INPUT);      // sets the digital pin 2 as input
  pinMode(int2Pin, INPUT);      // sets the digital pin 2 as input
}


void loop() {
 /* //Read both interrupt pins
  int int1 = digitalRead(int1Pin);
  int int2 = digitalRead(int2Pin);
  Serial.print(int1);
  Serial.print(",");
  Serial.println(int2);
  
 if (digitalRead(int1Pin) == 0 && digitalRead(int2Pin) == 0){
   //if both are low write
   writeRegister(0x30, 0x0A);
 }
 else {
   digitalWrite(pin, HIGH);
   int intEvent = readRegister(0x31);
   Serial.println(intEvent);
   delay(2000);
 }
 */
 int res = readRegister(0x0F); //WHO_AM_I
  Serial.println(res);
 //Serial.println(readRegister(0x28));
delay(100);
}
