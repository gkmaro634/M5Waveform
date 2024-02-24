#include <Arduino.h>
#include <M5Unified.h>

#define WIDTH 135
#define HEIGHT 240
#define DELAY 1000

#define YAXIS_DIV_LABLE_WIDTH 40
#define YAXIS_DIV_LABLE_HEIGHT 32
#define YAXIS_POS_LABLE_WIDTH 40
#define YAXIS_POS_LABLE_HEIGHT 16
#define XAXIS_DIV_LABLE_WIDTH 80
#define XAXIS_DIV_LABLE_HEIGHT 16

M5GFX display;
M5Canvas canvas(&display);

int count = 0;

void setup() {
  M5.begin();
  display.init();

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  canvas.createSprite(200, 96);

  display.fillScreen(BLACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (count % 2 == 0){
    canvas.fillSprite(BLACK);
  }
  else{
    canvas.fillSprite(BLUE);
  }

  display.startWrite();
  canvas.setPivot(100, 48);
  canvas.pushRotateZoom(67.5, 120, 90, 1, 1);//pushSprite(8, 8);
  display.endWrite();

  count++;
  delay(DELAY);
}
