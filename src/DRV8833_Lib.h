//**********************************************************************************************//
//					DEFINES						 	//
//**********************************************************************************************//

// wired connections
#define HG7881_B_IA 10                                 // 10 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 9                                // 9 --> Motor B Input B --> MOTOR B -
#define MOTOR_B_SLP                     8             // bring high to activate; will be pulled low

// functional connections
#define MOTOR_B_PWM HG7881_B_IA                       // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB                       // Motor B Direction

#define         MOTOR_DUTY_BUFFER       80            // Inputs with absolute value below this number are ignored
                                                      // Anyways the motor can't anything worthwhile with such less power
                           
//**********************************************************************************************//
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void motor_init();                                   // initialize motor pins
void motorOut(int rotationalVelocity);               //Sets motor rotational velocity

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//

// Motor initialization
void motor_init()			         
{
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  pinMode( MOTOR_B_SLP, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  digitalWrite( MOTOR_B_SLP, HIGH);
}

// actual motor driver function
void motorOut(int rotationalVelocity){
  int dutyCycle = rotationalVelocity;
  
  if (dutyCycle > MOTOR_DUTY_BUFFER)
  {analogWrite(MOTOR_B_PWM, dutyCycle);analogWrite(MOTOR_B_DIR, 0);Serial.println("forwards");}
  
  else if (dutyCycle < -MOTOR_DUTY_BUFFER)
  {analogWrite(MOTOR_B_DIR, -dutyCycle);analogWrite(MOTOR_B_PWM, 0);Serial.println("backwards");}
  
  else
  {analogWrite(MOTOR_B_PWM, 0);analogWrite(MOTOR_B_DIR, 0);Serial.println("stop");}
}

void motorTime(int time){
  if (time == 0) {
    motorOut(0);
  }
  else if (time > 0) {
    motorOut(255);
    delay(time);
    motorOut(0);
  }
  else if (time < 0) {
    motorOut(-255);
    delay(-time);
    motorOut(0);
  }
}
