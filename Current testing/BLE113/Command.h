/*
Commands Supported:

CFG LockedAngle;
CFG UnlockedAngle;
CFG Power=[80-255];

CMD LockCMD=[Lock/Unlock];

LOCKSTATUS;

user sets the lock to locked position and does CFG LockedAngle;
user sets the lock to unlocked position and does CFG UnlockedAngle;
the lock figures out the direction of lock/unlock.

*/

//**********************************************************************************************//
//					GLOBAL VARIABLES				 	//
//**********************************************************************************************//


//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//


void executeCommandFromUser(String inputString)
{
String command = "";
String value = "";
float angleFromAccelerometer = getAngle();

if (commandAvailable) {
    boolean stringOK = false;
    if (inputString.startsWith("CFG ")) 
    {
      inputString = inputString.substring(4);

      int pos = inputString.indexOf('=');
        if (pos > -1) 
        {
          command = inputString.substring(0, pos);
          value = inputString.substring(pos+1, inputString.length()-1);  // extract command up to ; excluded
          if (command.equals("Power")) { // set the power
              theMotorConfig.power = value.toInt();
               debug.println(command);
               debug.println(value);
              stringOK = true;
          }
        }
        else
        { 
          int pos = inputString.indexOf(';');
          if (pos > -1) {
          command = inputString.substring(0, pos);
          debug.println(command);
          
          if (command.equals("LockedAngle")) { // set the LockedAngle
            theMotorConfig.lockedAngle = angleFromAccelerometer;
            //debug.println("locked angle:"); debug.println(theMotorConfig.lockedAngle);  
            stringOK = true;
          }
          else if (command.equals("UnlockedAngle")) { // set the UnlockedAngle
            theMotorConfig.unlockedAngle = angleFromAccelerometer;
            //debug.println("unlocked angle:");  debug.println(theMotorConfig.unlockedAngle);
            stringOK = true;
          }
          
        }
      }
    }
    else if (inputString.startsWith("CMD ")) {
      inputString = inputString.substring(4);
      int pos = inputString.indexOf('=');
      if (pos > -1) {
        command = inputString.substring(0, pos);
        value = inputString.substring(pos+1, inputString.length()-1);  // extract command up to ; excluded
         debug.println(command);
         debug.println(value);
        if (command.equals("LockCMD")) { // lock or unlock 
            value.equals("Lock") ? controlMotor('l') : controlMotor('u'); 
          stringOK = true;
        }
    }
    }
    else if (inputString.startsWith("LOCKSTATUS")) {
        if(isLocked)
          debug.println("Locked");
        else
          debug.println("Unlocked");  
        stringOK = true;
      }
    
    // stringOK ? debug.println("Command Executed") : debug.println("Invalid Command");
    // clear the string for next iteration
    inputString = "";
    commandAvailable = false;
  } // stringComplete
  

}

