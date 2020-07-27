#include "freertos/task.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
// 利用ピンの定義
#define SERIAL_OUT   23
#define SHIFT_CLK    18
#define LATCH_CLK    16
int i = 0;
//データ格納
byte i_b[160] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
byte i_b_f[160];
//来ている者
byte incoming;
byte incomingbyte[100];
//出すもの
byte goal[10][16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float timer_1 = 0;
float timer_2 = 0;
float timer_s = 0;
// シフトレジスタへの8ビットデータのセット
void shift_dataOut(uint8_t data) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(SHIFT_CLK, LOW);
    digitalWrite(SERIAL_OUT, (data & (0x80 >> i)));
    digitalWrite(SHIFT_CLK, HIGH);
  }
}
// シフトレジスタからストレージレジスタへのデータセット
void strage_update() {
  digitalWrite(LATCH_CLK, LOW);
  digitalWrite(LATCH_CLK, HIGH);
}
void setup() {
  // put your setup code here, to run once:
  xTaskCreatePinnedToCore(shiftv, "shiftv", 8192, NULL, 20, NULL, 0);
  Serial.begin(921600);
  pinMode(SERIAL_OUT, OUTPUT);
  pinMode(SHIFT_CLK,  OUTPUT);
  pinMode(LATCH_CLK,  OUTPUT);
  digitalWrite(SHIFT_CLK, LOW);
  digitalWrite(LATCH_CLK, LOW);
  strage_update(); // シフトレジスタからストレージレジスタへの値セット
  delay(50);
}
bool ike = 0;
void shiftv(void *pvParameters) {
  while (1) {
    feedTheDog();
    if (Serial.available() > 11)
    {
      Serial.readBytes(incomingbyte, 12);
      for (int po = 0; po < 12; po++)
      {
        i_b_f[po] = incomingbyte[po];
        i_b[po * 2] = (i_b_f[po] & 0xF0) >> 4;
        i_b[(po * 2) + 1] = i_b_f[po] & 0x0F;
      }
    }
  }
}
void feedTheDog() {
  TIMERG0.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed = 1;
  TIMERG0.wdt_wprotect = 0;
  TIMERG1.wdt_wprotect = TIMG_WDT_WKEY_VALUE;
  TIMERG1.wdt_feed = 1;
  TIMERG1.wdt_wprotect = 0;
}

void loop() {
  for (int k = 0; k < 16; k++)
  {
    for (int n = 0; n < 10; n++)
    {
      goal[n][k] = ((i_b[0 + (n * 8)] != 0) << 7) + ((i_b[1 + (n * 8)] != 0) << 6) + ((i_b[2 + (n * 8)] != 0) << 5) + ((i_b[3 + (n * 8)] != 0) << 4) + ((i_b[4 + (n * 8)] != 0) << 3) + ((i_b[5 + (n * 8)] != 0) << 2) + ((i_b[6 + (n * 8)] != 0) << 1) + (i_b[7 + (n * 8)] != 0);
    }
    for (int p = 0; p < 80; p++)
    {
      if (i_b[p] > 0x00) 
      {
        i_b[p] = i_b[p] - 0x01;
      } else if (i_b[p] <= 0x00) 
      {
        i_b[p] = 0x00;
      }
    }
  }
  
  for (int as = 0; as < 10; as++) 
  {
    for (int ak = 0; ak < 16; ak++) 
    {
      for (int s = 0; s < 3; s++)
      {
        shift_dataOut(goal[s][ak]);
      }
      strage_update();            //ここでupdate
    }
  }
  for (int oi = 0; oi < 10; oi++) 
  {
    for (int ak = 0; ak < 16; ak++) 
    {
      for (int s = 0; s < 3; s++)
      {
        shift_dataOut(0x00);
      }
      strage_update();
    }
  }
}
