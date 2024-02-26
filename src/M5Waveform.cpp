#include "M5Waveform.hpp"

using namespace m5wf::figure_constants;

namespace m5wf
{
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

    _canvas.createSprite(width, height);
    _canvas.setPivot(_figureWidth / 2, _figureHeight / 2);
    _canvas.setColorDepth(8);

    _waveSprite.createSprite(_waveRegionWidth, _waveRegionHeight);
    _waveSprite.setPivot(_waveRegionWidth / 2, _waveRegionHeight / 2);
    _waveSprite.setColorDepth(8);

    // DEBUG
    _canvas.drawRect(
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
    _drawXAxisPosLabel();
    _drawYAxisDivLabel();
    _drawYAxisPosLabel();
  }

  void M5Waveform::drawWaveform(point_f *points, uint16_t length)
  {
    _waveSprite.fillScreen(BLACK);

    // point
    uint16_t x_pt, y_pt;
    for (int i = 0; i < length; i++)
    {
      auto p = points[i];
      if (_point2px(p, &x_pt, &y_pt) == 0)
      {
        _waveSprite.fillCircle(x_pt, y_pt, 2, WHITE);
      }
    }

    // line
    // for (int i = 1; i < length; i++)
    // {
    //   auto p1 = points[(i - 1) * sizeof(point_f)];
    //   auto p2 = points[i * sizeof(point_f)];
    // }

    _waveSprite.pushSprite(&_canvas, (int32_t)_waveRegionX, (int32_t)_waveRegionY, BLACK);
  }

  // RTC_TimeTypeDef TimeStruct;
  // void M5Waveform::add_value(float value)
  // {
  // // M5.Rtc.GetTime(&TimeStruct);
  // // auto dt = M5.Rtc.getDateTime();

  // // struct tm timeInfo;
  // // getLocalTime(&timeInfo);
  // // display.printf("%d\n", dt.time.seconds);
  // }

  uint8_t M5Waveform::_point2px(point_f point, uint16_t *x_px, uint16_t *y_px)
  {
    float xStart = (float)_xAxisPos;
    float xEnd = (float)_xAxisPos + (float)_xAxisDiv * ((float)_xAxisDivCount + (float)1);
    float yStart = (float)_yAxisPos;
    float yEnd = (float)_yAxisPos + (float)_yAxisDiv * ((float)_yAxisDivCount + (float)1);

    if (point.x < xStart || xEnd < point.x || point.y < yStart || yEnd < point.y)
    {
      return 1;
    }

    float dx = (float)_waveRegionWidth / (xEnd - xStart);
    float dy = (float)_waveRegionHeight / (yEnd - yStart);
    *x_px = (uint16_t)((point.x - xStart) * dx);
    *y_px = (uint16_t)(_waveRegionHeight - (point.y - yStart) * dy);

    return 0;
  }

  void M5Waveform::_drawXAxisRulerLine(void)
  {
    float step = (float)_waveRegionWidth / (float)(_xAxisDivCount + 1);
    for (int i = 0; i < _xAxisDivCount; i++)
    {
      float xPos = step * (i + 1) + _waveRegionX;
      _drawDashedLine((int)xPos, _waveRegionY, (int)xPos, _waveRegionY + _waveRegionHeight, 6, 2, BLUE);
    }
  }

  void M5Waveform::_drawYAxisRulerLine(void)
  {
    float step = (float)_waveRegionHeight / (float)(_yAxisDivCount + 1);
    for (int i = 0; i < _yAxisDivCount; i++)
    {
      float yPos = step * (i + 1) + _waveRegionY;
      _drawDashedLine(_waveRegionX, (int)yPos, _waveRegionX + _waveRegionWidth, (int)yPos, 6, 2, BLUE);
    }
  }

  void M5Waveform::_drawWaveformBorder(void)
  {
    _canvas.drawRect(
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
      _canvas.drawRect(MARGIN, _waveRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    case Y_POS:
      _canvas.drawRect(MARGIN, _waveRegionY + _waveRegionHeight - YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, BLUE);
      break;
    case X_DIV:
      _canvas.drawRect(_waveRegionX + _waveRegionWidth - XAXIS_DIV_LABLE_WIDTH, _waveRegionY + _waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    case X_POS:
      _canvas.drawRect(_waveRegionX, _waveRegionY + _waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, BLUE);
      break;
    default:
      break;
    }
  }

  void M5Waveform::_drawYAxisDivLabel(void)
  {
    _canvas.setCursor(MARGIN + MARGIN, _waveRegionY + MARGIN);
    _canvas.printf("%d", _yAxisDiv);
    _canvas.setCursor(MARGIN + MARGIN, _waveRegionY + MARGIN + LABEL_HEIGHT);
    _canvas.printf("/div");
  }

  void M5Waveform::_drawYAxisPosLabel(void)
  {
    _canvas.setCursor(MARGIN + MARGIN, _waveRegionY + _waveRegionHeight - YAXIS_POS_LABLE_HEIGHT + MARGIN);
    _canvas.printf("%d", _yAxisPos);
  }

  void M5Waveform::_drawXAxisDivLabel(void)
  {
    _canvas.setCursor(_waveRegionX + _waveRegionWidth - XAXIS_DIV_LABLE_WIDTH + MARGIN, _waveRegionY + _waveRegionHeight + MARGIN);
    _canvas.printf("%d /div", _xAxisDiv);
  }

  void M5Waveform::_drawXAxisPosLabel(void)
  {
    _canvas.setCursor(_waveRegionX + MARGIN, _waveRegionY + _waveRegionHeight + MARGIN);
    _canvas.printf("%d", _xAxisPos);
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
        _canvas.drawLine(x, y, x + dx * segmentLength / (segmentLength + spaceLength), y + dy * segmentLength / (segmentLength + spaceLength), color);
      }
      x += dx;
      y += dy;
    }
  }
}