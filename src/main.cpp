#include <Arduino.h>
#include <M5Unified.h>
#include "M5Plot.hpp"
#include "TimeSeriesData.hpp"
#include "M5Waveform.hpp"

#define WIDTH 135
#define HEIGHT 240
#define DELAY 500

M5GFX display;
M5Plot m5plot(&display);
M5Waveform wf(&m5plot);

int count = 0;

int32_t chartWidth = 200;
int32_t chartHeight = 96;

M5Plot::point_f points[8];

// TimeSeriesData tsData;

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(9600);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  m5plot.init(chartWidth, chartHeight, 4, 3);
  wf.init(8);
  wf.startDrawing();

  display.fillScreen(BLACK);

  // ダミーの点群を生成
  for (int i = 0; i < 8; i++)
  {
    int rand = (100 * i) % 41;
    points[i].x = (float)i * 20.0;
    points[i].y = (float)rand;
  }
}

void loop()
{
  auto ret = wf.enqueue(points[count%8].y);

  display.startWrite();

  // // 波形描画更新処理
  // m5plot.drawWaveform(points, 8, M5Plot::LINE);
  m5plot.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1);

  display.endWrite();

  count++;
  delay(DELAY);
  Serial.printf("%d\r\n", count);
}
