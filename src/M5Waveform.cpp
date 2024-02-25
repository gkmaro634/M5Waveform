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

  void M5Waveform::init(int32_t width, int32_t height, uint8_t xAxisDivCount, uint8_t yAxisDivCount)
  {
    _xAxisDivCount = xAxisDivCount;
    _yAxisDivCount = yAxisDivCount;

    M5Waveform::init(width, height);
  }

  void M5Waveform::init(int32_t width, int32_t height)
  {
    _figureWidth = width;
    _figureHeight = height;

    _waveRegionWidth = _figureWidth - MARGIN - MARGIN - YAXIS_DIV_LABLE_WIDTH;
    _waveRegionHeight = _figureHeight - MARGIN - MARGIN - XAXIS_DIV_LABLE_HEIGHT;

    figureCanvas->createSprite(width, height);
    figureCanvas->setPivot(_figureWidth / 2, _figureHeight / 2);
    figureCanvas->setColorDepth(8);

    // DEBUG
    figureCanvas->drawRect(
        0,
        0,
        _figureWidth,
        _figureHeight,
        WHITE);

    // 分割点線
    _drawXAxisRulerLine();
    _drawYAxisRulerLine();

    // 枠
    _drawWaveformBorder();

    // 選択枠
    _drawParamEditBorder();

    // 軸ラベル
    _drawXAxisDivLabel();
    _drawYAxisDivLabel();
    _drwaYAxisPosLabel();
  }

// RTC_TimeTypeDef TimeStruct;
  void M5Waveform::add_value(float value)
  {
  // M5.Rtc.GetTime(&TimeStruct);
  // auto dt = M5.Rtc.getDateTime();

  // struct tm timeInfo;
  // getLocalTime(&timeInfo);
  // display.printf("%d\n", dt.time.seconds);
  }

  void M5Waveform::_drawXAxisRulerLine(void)
  {
    for (int i = 0; i < _xAxisDivCount; i++)
    {
      auto xPos = _waveRegionWidth / (_xAxisDivCount + 1) * (i + 1) + _waveRegionX;
      _drawDashedLine(xPos, _waveRegionY, xPos, _waveRegionY + _waveRegionHeight, 6, 2, BLUE);
    }
  }

  void M5Waveform::_drawYAxisRulerLine(void)
  {
    for (int i = 0; i < _yAxisDivCount; i++)
    {
      auto yPos = _waveRegionHeight / (_yAxisDivCount + 1) * (i + 1) + _waveRegionY;
      _drawDashedLine(_waveRegionX, yPos, _waveRegionX + _waveRegionWidth, yPos, 6, 2, BLUE);
    }
  }

  void M5Waveform::_drawWaveformBorder(void)
  {
    figureCanvas->drawRect(
        _waveRegionX,
        _waveRegionY,
        _waveRegionWidth,
        _waveRegionHeight,
        BLUE);
  }

  void M5Waveform::_drawParamEditBorder(void)
  {
    switch (_selection)
    {
    case NOT_EDIT:
      break;
    case Y_DIV:
      figureCanvas->drawRect(MARGIN, _waveRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    case Y_POS:
      figureCanvas->drawRect(MARGIN, _waveRegionY + _waveRegionHeight - YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, BLUE);
      break;
    case X_DIV:
      figureCanvas->drawRect(_waveRegionX + _waveRegionWidth - XAXIS_DIV_LABLE_WIDTH, _waveRegionY + _waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    default:
      break;
    }
  }

  void M5Waveform::_drawYAxisDivLabel(void)
  {
    figureCanvas->setCursor(MARGIN + MARGIN, _waveRegionY + MARGIN);
    figureCanvas->printf("%d", _yAxisDiv);
    figureCanvas->setCursor(MARGIN + MARGIN, _waveRegionY + MARGIN + LABEL_HEIGHT);
    figureCanvas->printf("/div");
  }

  void M5Waveform::_drwaYAxisPosLabel(void)
  {
    figureCanvas->setCursor(MARGIN + MARGIN, _waveRegionY + _waveRegionHeight - YAXIS_POS_LABLE_HEIGHT + MARGIN);
    figureCanvas->printf("%d", _yAxisPos);
  }

  void M5Waveform::_drawXAxisDivLabel(void)
  {
    figureCanvas->setCursor(_waveRegionX + _waveRegionWidth - XAXIS_DIV_LABLE_WIDTH + MARGIN, _waveRegionY + _waveRegionHeight + MARGIN);
    figureCanvas->printf("%d /div", _xAxisDiv);
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