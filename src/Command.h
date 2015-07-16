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

void executeCommandFromUser(String inputString){
  int ID = inputString[0];
  debugPrint("ID     : ");debugPrintlnInt(ID);
  if (hasPermissions(ID) == 0){return;}

  String command = inputString.substring(1,4);
  String input = inputString.substring(4,inputString.length());

  debugPrint("Command: ");debugPrintln(command);
  debugPrint("Input  : ");debugPrintln(input);

  if (command == "CFG"){
    String settingString = input.substring(0,3);
    String value = input.substring(3, inputString.length());

    Attribute setting;
    
    if (settingString == "PWR"){setting = settings.power;}
    else if (settingString == "LKA"){setting = settings.lockedAngle;}
    else if (settingString == "ULA"){setting = settings.unlockedAngle;}
    else if (settingString == "PAS"){setting = settings.lockPass;}

    setting.setData(value.toInt());
    debugPrint("Set ");debugPrint(settingString);debugPrint(" to ");debugPrintlnInt(value.toInt());
  }
  
  else if (command == "CMD"){
    if      (input.equals("LCK")){controlMotor('l');}
    else if (input.equals("ULK")){controlMotor('u');}
    else if (input.equals("LKA")){settings.lockedAngle.setData(getAngle());}
    else if (input.equals("ULA")){settings.unlockedAngle.setData(getAngle());}
  }
  else if (command == "STS"){
    if(isLocked)
      debugPrintln("Locked");
    else
      debugPrintln("Unlocked");  
  }
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

