bool hasPermissions(byte ID){
  if (ID == settings.lockPass.getData()){return 1;}
  else if (ID == 97){return 1;}
  return 0;
}

