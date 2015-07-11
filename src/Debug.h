

void debug_init(){
  Serial.begin(38400);
  Serial.println("Serial online");
}

String readSerial(){
  String outputString = "";
  if (Serial.available()){
    while (Serial.available()) {
      char inChar = (char)Serial.read();
      outputString += inChar;
    }
  }
  if(outputString.length() > 0)
  {Serial.print("Got String: "); Serial.println(outputString);}
  return outputString;
}
