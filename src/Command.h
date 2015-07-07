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
//				   FUNCTIONS DECLARATIONS				 	//
//**********************************************************************************************//

void executeCommandFromUser(String inputString);
void debugCommandFromUser(String inputString);

//**********************************************************************************************//
//					FUNCTIONS					 	//
//**********************************************************************************************//


void executeCommandFromUser(String inputString)
{
String command = "";
String value = "";

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
            settings.power.setData(value.toInt());
             Serial.println(command);
             Serial.println(value);
            stringOK = true;
        }
      }
      else
      { 
        int pos = inputString.indexOf(';');
        if (pos > -1) {
        command = inputString.substring(0, pos);
        Serial.println(command);
        
        if (command.equals("LockedAngle")) { // set the LockedAngle
          settings.lockedAngle.setData(getAngle());
          stringOK = true;
        }
        else if (command.equals("UnlockedAngle")) { // set the UnlockedAngle
          settings.unlockedAngle.setData(getAngle());
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
       Serial.println(command);
       Serial.println(value);
      if (command.equals("Lock")) { // lock or unlock 
          value.equals("Lock") ? controlMotor('l') : controlMotor('u'); 
        stringOK = true;
      }
  }
  }
  else if (inputString.startsWith("LOCKSTATUS")) {
      if(isLocked)
        Serial.println("Locked");
      else
        Serial.println("Unlocked");  
      stringOK = true;
    }
  
  // clear the string for next iteration
  inputString = "";
  commandAvailable = false;
} // stringComplete


void debugCommandFromUser(String inputString){
  if (inputString == "c"){controlMotor('l');}
  else if (inputString == "o"){controlMotor('u');}
  else if (inputString == "l"){
    motorTime(settings.turnTestTime.getData()*10+300);
    motorTime(-settings.turnTestTime.getData()*10);
  }
  else if (inputString == "r"){
    motorTime(-settings.turnTestTime.getData()*10-300);
    motorTime(settings.turnTestTime.getData()*10);
  }
  else if (inputString == "i"){
    settings.turnTestTime.setData(settings.turnTestTime.getData()+10);
  }
  else if (inputString == "d"){
    settings.turnTestTime.setData(settings.turnTestTime.getData()-10);
  }
}

