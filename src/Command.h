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

void motorTime(int duration) {
	if (duration == 0) {
		return;
	}
	if (duration > 0) {
		gMotorController.cmdDriveMotorForDuration(MotorController::CW, 255, duration);
	}
	else {
		gMotorController.cmdDriveMotorForDuration(MotorController::CCW, 255, -duration);
	}
}
void executeCommandFromUser(String inputString){
  int ID = inputString[0];
	debugPrint(F("ID     : "));debugPrintlnInt(ID);
  if (hasPermissions(ID) == 0){return;}

  String command = inputString.substring(1,4);
  String input = inputString.substring(4,inputString.length());

	debugPrint(F("Command: "));debugPrintln(command);
	debugPrint(F("Input  : "));debugPrintln(input);

  if (command == "CFG"){
    String settingString = input.substring(0,3);
    String value = input.substring(3, inputString.length());

    Attribute setting;
    
    if (settingString == "PWR"){setting = settings.power;}
    else if (settingString == "LKA"){setting = settings.lockedAngle;}
    else if (settingString == "ULA"){setting = settings.unlockedAngle;}
    else if (settingString == "PAS"){setting = settings.lockPass;}

    setting.setData(value.toInt());
		debugPrint(F("Set "));debugPrint(settingString);debugPrint(F(" to "));debugPrintlnInt(value.toInt());
  }
  
  else if (command == "CMD"){
		if      (input.equals("LCK")){gLockSystemController.cmdLock();}
		else if (input.equals("ULK")){gLockSystemController.cmdUnlock();}
		/*
		else if (input.equals("LKA")){settings.lockedAngle.setData(gLockAccelerometerObserver.getLockAngleDeg());}
		else if (input.equals("ULA")){settings.unlockedAngle.setData(gLockAccelerometerObserver.getLockAngleDeg());}
		*/
		else if (input.equals("LKA")) { gLockSystemController.cmdCalibAtLockedPos(); }
		else if (input.equals("ULA")) { gLockSystemController.cmdCalibAtUnlockedPos(); }
		else {
			debugPrint(F("input length "));debugPrintlnInt(input.length());
		}
  }
  else if (command == "STS"){
		gLockSystemController.stateInfo();
  }
} // stringComplete

void debugCommandFromUser(String inputString){
	if (inputString == "c"){
		gLockSystemController.cmdLock();
	}
	else if (inputString == "o"){
		//controlMotor('u');
		gLockSystemController.cmdUnlock();
	}
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

