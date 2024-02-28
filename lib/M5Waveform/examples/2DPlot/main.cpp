
#include <Arduino.h>
#include <M5Unified.h>
#include "M5Plot.hpp"

#define WIDTH 135
#define HEIGHT 240
#define DELAY 3000

M5GFX display;
M5Plot m5plot(&display);

int count = 0;

int32_t chartWidth = 200;
int32_t chartHeight = 96;

m5wf::point_f points[8];

void setup()
{
  M5.begin();
  display.init();
  Serial.begin(9600);
  Serial.println("Device initialized.");

  // 全体領域のうち波形に割り当てる領域サイズを指定する
  m5plot.init(chartWidth, chartHeight, 4, 3);

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
    m5plot.setEditTarget(m5wf::Y_DIV);
  }
  else if ((count / 4) % 4 == 1){
    m5plot.setEditTarget(m5wf::Y_POS);
  }
  else if ((count / 4) % 4 == 2){
    m5plot.setEditTarget(m5wf::X_POS);
  }
  else if ((count / 4) % 4 == 3){
    m5plot.setEditTarget(m5wf::X_DIV);
  }

  // 軸設定状態切り替えの模擬
  if (count % 3 == 0)
  {
    m5plot.setEditState(m5wf::NOT_EDIT);
    // m5plot.updateXAxisDiv(20);
    // m5plot.updateXAxisPos(20);
    // m5plot.updateYAxisDiv(20);
    // m5plot.updateYAxisPos(20);
  }
  else if(count % 3 == 1)
  {
    m5plot.setEditState(m5wf::SELECT);
  }
  else
  {
    m5plot.setEditState(m5wf::EDIT);
    // m5plot.updateXAxisDiv(30);
    // m5plot.updateXAxisPos(30);
    // m5plot.updateYAxisDiv(30);
    // m5plot.updateYAxisPos(30);
  }

  display.startWrite();

  // 波形描画更新処理
  m5plot.plot(points, 8, m5wf::LINE);
  m5plot.figureCanvas->pushRotateZoom(67.5, 120, 90, 1, 1);

  display.endWrite();

  count++;
  delay(DELAY);
  Serial.printf("%d\r\n", count);
}
