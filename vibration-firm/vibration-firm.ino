void setup() {
  // put your setup code here, to run once:
Serial.begin(921600);
}

void loop() {
  byte buf = 0;
  // put your main code here, to run repeatedly:  
  if(Serial.available() > 0){
    buf = Serial.read();
    dacWrite(25, buf);
  }
}
