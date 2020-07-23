// 利用ピンの定義
#define SERIAL_OUT   23
#define SHIFT_CLK    18
#define LATCH_CLK    16

// シフトレジスタへの8ビットデータのセット
void shift_dataOut(uint8_t data) {
  for (uint8_t i=0; i < 8; i++) {
    digitalWrite(SHIFT_CLK,LOW);
    digitalWrite(SERIAL_OUT, (data & (0x80>>i)));
    digitalWrite(SHIFT_CLK,HIGH);
  }
}
// シフトレジスタからストレージレジスタへのデータセット
void strage_update() {
  digitalWrite(LATCH_CLK,LOW);
  digitalWrite(LATCH_CLK,HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600);
  pinMode(SERIAL_OUT, OUTPUT);
  pinMode(SHIFT_CLK,  OUTPUT);
  pinMode(LATCH_CLK,  OUTPUT);

  digitalWrite(SHIFT_CLK,LOW);
  digitalWrite(LATCH_CLK,LOW);
  strage_update(); // シフトレジスタからストレージレジスタへの値セット
  delay(50);
}

int i = 0;
  //データ格納
byte incomingbyte[8] = {0,0,0,0,0,0,0,0};
byte incoming;

byte incoming_s[8];
byte finalout[8];

void loop() {
  // put your main code here, to run repeatedly:
  
  byte check_last = 0xFF;
  if (Serial.available() > 0) {
    incoming = Serial.read();
    if(check_last == incoming){
      i = 0;
    }else{
      incomingbyte[i++] = incoming;
    }
  }
  //Serial.printf("%d", i);

   
  for(int n = 0; n < 64; n++)
  {
    for(int s = 0; s < 8; s++)
    {
      if(incomingbyte[s] != 0)
        {
          incoming_s[s] = 0x01;
                incomingbyte[s] = incomingbyte[s] - 1;
        }else if(incoming_s[s] == 0){
          incoming_s[s] = 0x00;  
                incomingbyte[s] = incomingbyte[s] - 1;
        }
      finalout[0] = incoming_s[s];
      if(s != 7) finalout[0] = finalout[0] << 1;  
      //Serial.println(finalout[0]);
    }
  
    for(int k = 0; k < 1; k++)
    {
      shift_dataOut(finalout[k]);  
    }
    strage_update();
  }
  for(int n = 0; n < 64; n++)
  {
    for(int k = 0; k < 1; k++)
    {
    shift_dataOut(0x00);
    }  
    strage_update();
  }
}
