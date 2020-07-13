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

void loop() {
  byte buf = 0;
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    buf = Serial.read();
    dacWrite(25, buf);
  }
}
