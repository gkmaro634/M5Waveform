#include <random>

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

// 乱数用
std::random_device rd;
std::mt19937 gen;
std::uniform_real_distribution<float> dis(0, 40.0);

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(9600);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  wf.init(chartWidth, chartHeight, 4, 3);
  wf.updateXAxisDiv(2);
  wf.updateYAxisDiv(10);
  wf.startDrawing(8, onDrawingCallback);

  display.fillScreen(BLACK);

  gen.seed(rd());  
}

void loop()
{
  float randValue = dis(gen);
  auto ret = wf.enqueue(randValue);

  delay(DELAY);
  Serial.printf("%.1f\r\n", randValue);
}

void onDrawingCallback(void)
{
  // 波形描画更新処理
  display.startWrite();

  wf.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1);

  display.endWrite();
}