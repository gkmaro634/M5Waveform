#include <Arduino.h>
#include <M5Unified.h>

#define WIDTH 135
#define HEIGHT 240
#define DELAY 1000

#define MARGIN 4
#define LABEL_HEIGHT 16
#define YAXIS_DIV_LABLE_WIDTH 40
#define YAXIS_DIV_LABLE_HEIGHT LABEL_HEIGHT*2
#define YAXIS_POS_LABLE_WIDTH 40
#define YAXIS_POS_LABLE_HEIGHT LABEL_HEIGHT
#define XAXIS_DIV_LABLE_WIDTH 80
#define XAXIS_DIV_LABLE_HEIGHT LABEL_HEIGHT

typedef enum {
  Y_DIV = 1,
  Y_POS,
  X_DIV

} EditSelection;

void drawDashedLine(M5Canvas canvas, int x0, int y0, int x1, int y1, int segmentLength, int spaceLength, uint16_t color);

M5GFX display;
M5Canvas canvas(&display);

int count = 0;

uint8_t yAxisDivCount = 1;// 0は分割無し, 1で二等分, 2,3,...255
uint8_t xAxisDivCount = 4;// 0は分割無し, 1で二等分, 2,3,...255
uint16_t yAxisDiv = 20;
uint16_t yAxisPos = 100;
uint16_t xAxisDiv = 50;

int32_t chartWidth = 200;
int32_t chartHeight = 96;

EditSelection selection = Y_DIV;

void setup() {
  M5.begin();
  display.init();

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  canvas.createSprite(chartWidth, chartHeight);
  canvas.setColorDepth(8);

  // DEBUG
  canvas.drawRect(
    0, 
    0,
    chartWidth,
    chartHeight,
    WHITE);

  // 不変の部品を描画する
  auto waveRegionX = MARGIN + YAXIS_DIV_LABLE_WIDTH;
  auto waveRegionY = MARGIN;
  auto waveRegionWidth = chartWidth - MARGIN - MARGIN - YAXIS_DIV_LABLE_WIDTH;
  auto waveRegionHeight = chartHeight - MARGIN - MARGIN - XAXIS_DIV_LABLE_HEIGHT;

  // 分割点線
  // X div
  for (int i=0; i < xAxisDivCount; i++){
    auto xPos = waveRegionWidth / (xAxisDivCount + 1) * (i + 1) + waveRegionX;
    drawDashedLine(canvas, xPos, waveRegionY, xPos, waveRegionY + waveRegionHeight, 6, 2, BLUE);
  }
  // Y div
  for (int i=0; i < yAxisDivCount; i++){
    auto yPos = waveRegionHeight / (yAxisDivCount + 1) * (i + 1) + waveRegionY;
    drawDashedLine(canvas, waveRegionX, yPos, waveRegionX + waveRegionWidth, yPos, 6, 2, BLUE);
  }

  // 枠
  canvas.drawRect(
    waveRegionX, 
    waveRegionY,
    waveRegionWidth,
    waveRegionHeight,
    BLUE);

  // 選択枠
  switch (selection)
  {
  case Y_DIV:
    /* code */
    canvas.drawRect(MARGIN, waveRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, BLUE);
    break;
  case Y_POS:
    /* code */
    canvas.drawRect(MARGIN, waveRegionY+waveRegionHeight-YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, BLUE);
    break;
  case X_DIV:
    /* code */
    canvas.drawRect(waveRegionX+waveRegionWidth-XAXIS_DIV_LABLE_WIDTH, waveRegionY+waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, BLUE);
    break;
  default:
    break;
  }

  // 軸ラベル
  // Y div
  canvas.setCursor(MARGIN + MARGIN, waveRegionY + MARGIN);
  canvas.printf("%d", yAxisDiv);
  canvas.setCursor(MARGIN + MARGIN, waveRegionY + MARGIN + LABEL_HEIGHT);
  canvas.printf("/div");

  // Y pos
  canvas.setCursor(MARGIN + MARGIN, waveRegionY+waveRegionHeight-YAXIS_POS_LABLE_HEIGHT + MARGIN);
  canvas.printf("%d", yAxisPos);

  // X div
  canvas.setCursor(waveRegionX + waveRegionWidth - XAXIS_DIV_LABLE_WIDTH + MARGIN, waveRegionY+waveRegionHeight + MARGIN);
  canvas.printf("%d /div", xAxisDiv);

  // 描画設定
  canvas.setPivot(chartWidth/2, chartHeight/2);

  display.fillScreen(BLACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (count % 2 == 0){
  //   canvas.fillSprite(BLACK);
  // }
  // else{
  //   canvas.fillSprite(BLUE);
  // }

  display.startWrite();
  canvas.pushRotateZoom(67.5, 120, 90, 1, 1);//pushSprite(8, 8);
  display.endWrite();

  count++;
  delay(DELAY);
}

void drawDashedLine(M5Canvas canvas, int x0, int y0, int x1, int y1, int segmentLength, int spaceLength, uint16_t color) {
  float distance = sqrt(sq(x1 - x0) + sq(y1 - y0));
  float dashCount = distance / (segmentLength + spaceLength);
  float dx = (x1 - x0) / dashCount;
  float dy = (y1 - y0) / dashCount;
  float x = x0;
  float y = y0;
  for (int i = 0; i < dashCount; i++) {
    if (i % 2 == 0) { // セグメントを描画
      canvas.drawLine(x, y, x + dx * segmentLength / (segmentLength + spaceLength), y + dy * segmentLength / (segmentLength + spaceLength), color);
    }
    x += dx;
    y += dy;
  }
}