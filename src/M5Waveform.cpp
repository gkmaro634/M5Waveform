#include "M5Waveform.hpp"

using namespace m5wf::figure_constants;

namespace m5wf
{
  M5Waveform::M5Waveform()
  {
  }

  M5Waveform::M5Waveform(M5Canvas *canvas)
  {
    figureCanvas = canvas;
  }

  M5Waveform::~M5Waveform()
  {
    // memo: 本クラス内でnewした領域などはここで解放する
  }

  void M5Waveform::init(int32_t width, int32_t height)
  {
    _figureWidth = width;
    _figureHeight = height;

    figureCanvas->createSprite(width, height);
    figureCanvas->setColorDepth(8);

    // DEBUG
    figureCanvas->drawRect(
        0,
        0,
        _figureWidth,
        _figureHeight,
        WHITE);

    // 不変の部品を描画する
    auto waveRegionX = MARGIN + YAXIS_DIV_LABLE_WIDTH;
    auto waveRegionY = MARGIN;
    auto waveRegionWidth = _figureWidth - MARGIN - MARGIN - YAXIS_DIV_LABLE_WIDTH;
    auto waveRegionHeight = _figureHeight - MARGIN - MARGIN - XAXIS_DIV_LABLE_HEIGHT;

    // 分割点線
    // X div
    for (int i = 0; i < _xAxisDivCount; i++)
    {
      auto xPos = waveRegionWidth / (_xAxisDivCount + 1) * (i + 1) + waveRegionX;
      _drawDashedLine(xPos, waveRegionY, xPos, waveRegionY + waveRegionHeight, 6, 2, BLUE);
    }
    // Y div
    for (int i = 0; i < _yAxisDivCount; i++)
    {
      auto yPos = waveRegionHeight / (_yAxisDivCount + 1) * (i + 1) + waveRegionY;
      _drawDashedLine(waveRegionX, yPos, waveRegionX + waveRegionWidth, yPos, 6, 2, BLUE);
    }

    // 枠
    figureCanvas->drawRect(
        waveRegionX,
        waveRegionY,
        waveRegionWidth,
        waveRegionHeight,
        BLUE);

    // 選択枠
    switch (_selection)
    {
    case Y_DIV:
      /* code */
      figureCanvas->drawRect(MARGIN, waveRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    case Y_POS:
      /* code */
      figureCanvas->drawRect(MARGIN, waveRegionY + waveRegionHeight - YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, BLUE);
      break;
    case X_DIV:
      /* code */
      figureCanvas->drawRect(waveRegionX + waveRegionWidth - XAXIS_DIV_LABLE_WIDTH, waveRegionY + waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    default:
      break;
    }

    // 軸ラベル
    // Y div
    figureCanvas->setCursor(MARGIN + MARGIN, waveRegionY + MARGIN);
    figureCanvas->printf("%d", _yAxisDiv);
    figureCanvas->setCursor(MARGIN + MARGIN, waveRegionY + MARGIN + LABEL_HEIGHT);
    figureCanvas->printf("/div");

    // Y pos
    figureCanvas->setCursor(MARGIN + MARGIN, waveRegionY + waveRegionHeight - YAXIS_POS_LABLE_HEIGHT + MARGIN);
    figureCanvas->printf("%d", _yAxisPos);

    // X div
    figureCanvas->setCursor(waveRegionX + waveRegionWidth - XAXIS_DIV_LABLE_WIDTH + MARGIN, waveRegionY + waveRegionHeight + MARGIN);
    figureCanvas->printf("%d /div", _xAxisDiv);

    // 描画設定
    figureCanvas->setPivot(_figureWidth / 2, _figureHeight / 2);    
  }

  void M5Waveform::_drawDashedLine(int x0, int y0, int x1, int y1, int segmentLength, int spaceLength, uint16_t color)
  {
    float distance = sqrt(sq(x1 - x0) + sq(y1 - y0));
    float dashCount = distance / (segmentLength + spaceLength);
    float dx = (x1 - x0) / dashCount;
    float dy = (y1 - y0) / dashCount;
    float x = x0;
    float y = y0;
    for (int i = 0; i < dashCount; i++)
    {
      if (i % 2 == 0)
      { // セグメントを描画
        figureCanvas->drawLine(x, y, x + dx * segmentLength / (segmentLength + spaceLength), y + dy * segmentLength / (segmentLength + spaceLength), color);
      }
      x += dx;
      y += dy;
    }
  }
}