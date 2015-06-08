int TURN_TIME = 4000;
int count = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(9,OUTPUT);
pinMode(11,OUTPUT);
pinMode(13,OUTPUT);
Serial.begin(9600);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(9,LOW);
  digitalWrite(11,HIGH);
  digitalWrite(13,HIGH);
  delay(TURN_TIME);
  
  digitalWrite(9,HIGH);
  digitalWrite(11,LOW);
  digitalWrite(13,LOW);
  delay(TURN_TIME);
  
  count += 1;
  
  Serial.println(count);
}
