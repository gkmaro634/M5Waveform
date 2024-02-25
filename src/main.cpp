#include <Arduino.h>
#include <M5Unified.h>
#include "M5Waveform.hpp"
#include "time.h"

#define WIDTH 135
#define HEIGHT 240
#define DELAY 1000

M5GFX display;
m5wf::M5Waveform waveform(&display);

int count = 0;

// uint8_t yAxisDivCount = 1;// 0は分割無し, 1で二等分, 2,3,...255
// uint8_t xAxisDivCount = 4;// 0は分割無し, 1で二等分, 2,3,...255
// uint16_t yAxisDiv = 20;
// uint16_t yAxisPos = 100;
// uint16_t xAxisDiv = 50;

int32_t chartWidth = 200;
int32_t chartHeight = 96;

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(115200);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  waveform.init(chartWidth, chartHeight, 4, 3);

  display.fillScreen(BLUE);
}

void loop()
{
  // put your main code here, to run repeatedly:
  // if (count % 2 == 0){
  //   canvas.fillSprite(BLACK);
  // }
  // else{
  //   canvas.fillSprite(BLUE);
  // }

  display.startWrite();

  waveform.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1); // pushSprite(8, 8);

  display.endWrite();

  count++;
  delay(DELAY);
  Serial.printf("%d\r\n", count);
}
