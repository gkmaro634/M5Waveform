#include <Arduino.h>
#include <M5Unified.h>
#include "M5Waveform.hpp"
#include "time.h"

#define WIDTH 135
#define HEIGHT 240
#define DELAY 3000

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

m5wf::M5Waveform::point_f points[8];

// m5wf::M5Waveform::EditState editState = m5wf::M5Waveform::NOT_EDIT;
// m5wf::M5Waveform::EditTarget editTarget = m5wf::M5Waveform::X_DIV;

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(115200);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  waveform.init(chartWidth, chartHeight, 4, 3);

  display.fillScreen(BLACK);

  for (int i = 0; i < 8; i++)
  {
    int rand = (100 * i) % 41;
    points[i].x = (float)i * 20.0; // + 40.0;
    points[i].y = (float)rand;  // + 120.0;
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (count % 3 == 0)
  {
    waveform.setEditState(m5wf::M5Waveform::NOT_EDIT);
    // waveform.updateXAxisDiv(20);
    // waveform.updateXAxisPos(20);
    // waveform.updateYAxisDiv(20);
    // waveform.updateYAxisPos(20);
  }
  else if(count % 3 == 1)
  {
    waveform.setEditState(m5wf::M5Waveform::SELECT);
  }
  else
  {
    waveform.setEditState(m5wf::M5Waveform::EDIT);
    // waveform.updateXAxisDiv(30);
    // waveform.updateXAxisPos(30);
    // waveform.updateYAxisDiv(30);
    // waveform.updateYAxisPos(30);
  }

  display.startWrite();

  waveform.drawWaveform(points, 8);
  waveform.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1); // pushSprite(8, 8);

  display.endWrite();

  count++;
  delay(DELAY);
  Serial.printf("%d\r\n", count);
}
