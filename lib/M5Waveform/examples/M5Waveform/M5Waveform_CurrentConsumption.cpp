#include <Arduino.h>
#include <M5Unified.h>
#include "M5Waveform.hpp"

#define WIDTH 135
#define HEIGHT 240
#define DELAY 500

void onDrawingCallback(void);

M5GFX display;
M5Waveform wf(&display);

int32_t chartWidth = 200;
int32_t chartHeight = 96;

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(9600);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  wf.init(chartWidth, chartHeight, 4, 3);
  wf.updateXAxisDiv(2);
  wf.updateYAxisDiv(20);
  wf.startDrawing(8, onDrawingCallback, m5wf::LINE);

  display.fillScreen(BLACK);
}

void loop()
{
  float current = M5.Power.Axp192.getVBUSCurrent();
  auto ret = wf.enqueue(current);

  delay(DELAY);
}

void onDrawingCallback(void)
{
  // 波形描画更新処理
  display.startWrite();

  wf.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1);

  display.endWrite();
}