#include <Arduino.h>
#include <M5Unified.h>
#include "M5Waveform.hpp"

#define WIDTH 135
#define HEIGHT 240
#define DELAY 3000

M5GFX display;
m5wf::M5Waveform waveform(&display);

int count = 0;

int32_t chartWidth = 200;
int32_t chartHeight = 96;

m5wf::M5Waveform::point_f points[8];

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(9600);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  waveform.init(chartWidth, chartHeight, 4, 3);

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
  // 軸設定対象切り替えの模擬
  if ((count / 4) % 4 == 0)
  {
    waveform.setEditTarget(m5wf::M5Waveform::Y_DIV);
  }
  else if ((count / 4) % 4 == 1){
    waveform.setEditTarget(m5wf::M5Waveform::Y_POS);
  }
  else if ((count / 4) % 4 == 2){
    waveform.setEditTarget(m5wf::M5Waveform::X_POS);
  }
  else if ((count / 4) % 4 == 3){
    waveform.setEditTarget(m5wf::M5Waveform::X_DIV);
  }

  // 軸設定状態切り替えの模擬
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

  // 波形描画更新処理
  waveform.drawWaveform(points, 8, m5wf::M5Waveform::LINE);
  waveform.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1);

  display.endWrite();

  count++;
  delay(DELAY);
  Serial.printf("%d\r\n", count);
}
