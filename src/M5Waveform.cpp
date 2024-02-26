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

    _canvas.createSprite(_figureWidth, _figureHeight);
    _canvas.setColorDepth(8);
    _canvas.setPivot(_figureWidth / 2, _figureHeight / 2);

    _waveSprite.createSprite(_waveRegionWidth, _waveRegionHeight);
    _waveSprite.setColorDepth(8);

    _figureSprite.createSprite(_figureWidth, _figureHeight);
    _figureSprite.setColorDepth(8);

    // DEBUG
    // _figureSprite.drawRect(
    //     0,
    //     0,
    //     _figureWidth,
    //     _figureHeight,
    //     WHITE);

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

    _renderFigure();
  }

  void M5Waveform::drawWaveform(point_f *points, uint16_t length)
  {
    _clearCanvas();
    _clearWaveform();

    // point
    int x_pt, y_pt;
    for (int i = 0; i < length; i++)
    {
      auto p = points[i];
      if (_point2px(p, &x_pt, &y_pt) == 0)
      {
        _waveSprite.fillCircle(x_pt, y_pt, 2, WHITE);
      }
    }

    // line
    int x1_pt, y1_pt;
    int x2_pt, y2_pt;
    uint8_t ret1, ret2;
    for (int i = 1; i < length; i++)
    {
      auto p1 = points[i - 1];
      auto p2 = points[i];
      ret1 = _point2px(p1, &x1_pt, &y1_pt);
      ret2 = _point2px(p2, &x2_pt, &y2_pt);

      // 両方範囲外
      if (ret1 != 0 && ret2 != 0)
      {
        continue;
      }

      _waveSprite.drawLine(x1_pt, y1_pt, x2_pt, y2_pt, WHITE);
    }

    _renderFigure();
    _renderWaveform();
  }

  void M5Waveform::updateXAxisDiv(uint16_t value)
  {
    _xAxisDiv = value;
    _drawXAxisDivLabel();
    _renderFigure();
  }

  void M5Waveform::updateXAxisPos(uint16_t value)
  {
    _xAxisPos = value;
    _drawXAxisPosLabel();
    _renderFigure();
  }

  void M5Waveform::updateYAxisDiv(uint16_t value)
  {
    _yAxisDiv = value;
    _drawYAxisDivLabel();
    _renderFigure();
  }

  void M5Waveform::updateYAxisPos(uint16_t value)
  {
    _yAxisPos = value;
    _drawYAxisPosLabel();
    _renderFigure();
  }

  void M5Waveform::setEditState(EditState state)
  {
    _clearCanvas();

    _editState = state;
    _drawParamEditBorder();

    _renderFigure();
    _renderWaveform();
  }
  
  void M5Waveform::setEditTarget(EditTarget target)
  {
    _clearCanvas();

    _editTarget = target;
    _drawParamEditBorder();

    _renderFigure();
    _renderWaveform();
  }

  void M5Waveform::_renderFigure(void)
  {
    _figureSprite.pushSprite(&_canvas, 0, 0, BLACK);
  }

  void M5Waveform::_renderWaveform(void)
  {
    _waveSprite.pushSprite(&_canvas, (int32_t)_waveRegionX, (int32_t)_waveRegionY, BLACK);
  }

  void M5Waveform::_clearCanvas(void)
  {
    _canvas.fillScreen(BLACK);
  }

  void M5Waveform::_clearWaveform(void)
  {
    _waveSprite.fillScreen(BLACK);
  }

  void M5Waveform::_clearFigure(void)
  {
    _figureSprite.fillScreen(BLACK);
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

  uint8_t M5Waveform::_point2px(point_f point, int *x_px, int *y_px)
  {
    float xStart = (float)_xAxisPos;
    float xEnd = (float)_xAxisPos + (float)_xAxisDiv * ((float)_xAxisDivCount + (float)1);
    float yStart = (float)_yAxisPos;
    float yEnd = (float)_yAxisPos + (float)_yAxisDiv * ((float)_yAxisDivCount + (float)1);
    uint8_t ret = 0;

    if (point.x < xStart || xEnd < point.x || point.y < yStart || yEnd < point.y)
    {
      ret = 1;
    }

    float dx = (float)_waveRegionWidth / (xEnd - xStart);
    float dy = (float)_waveRegionHeight / (yEnd - yStart);
    *x_px = (int)((point.x - xStart) * dx);
    *y_px = (int)(_waveRegionHeight - (point.y - yStart) * dy);

    return ret;
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
    _figureSprite.drawRect(
        _waveRegionX,
        _waveRegionY,
        _waveRegionWidth,
        _waveRegionHeight,
        BLUE);
  }

  void M5Waveform::_drawParamEditBorder(void)
  {
    int borderColor = _editState == NOT_EDIT ? BLACK : _editState == SELECT ? BLUE : RED;

    switch (_editTarget)
    {
    case Y_DIV:
      _figureSprite.drawRect(MARGIN, _waveRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, borderColor);
      break;
    case Y_POS:
      _figureSprite.drawRect(MARGIN, _waveRegionY + _waveRegionHeight - YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, borderColor);
      break;
    case X_DIV:
      _figureSprite.drawRect(_waveRegionX + _waveRegionWidth - XAXIS_DIV_LABLE_WIDTH, _waveRegionY + _waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, borderColor);
      break;
    case X_POS:
      _figureSprite.drawRect(_waveRegionX, _waveRegionY + _waveRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, borderColor);
      break;
    default:
      break;
    }
  }

  void M5Waveform::_drawYAxisDivLabel(void)
  {
    _figureSprite.setCursor(MARGIN + MARGIN, _waveRegionY + MARGIN);
    _figureSprite.printf("%d", _yAxisDiv);
    _figureSprite.setCursor(MARGIN + MARGIN, _waveRegionY + MARGIN + LABEL_HEIGHT);
    _figureSprite.printf("/div");
  }

  void M5Waveform::_drawYAxisPosLabel(void)
  {
    _figureSprite.setCursor(MARGIN + MARGIN, _waveRegionY + _waveRegionHeight - YAXIS_POS_LABLE_HEIGHT + MARGIN);
    _figureSprite.printf("%5d", _yAxisPos);
  }

  void M5Waveform::_drawXAxisDivLabel(void)
  {
    _figureSprite.setCursor(_waveRegionX + _waveRegionWidth - XAXIS_DIV_LABLE_WIDTH + MARGIN, _waveRegionY + _waveRegionHeight + MARGIN);
    _figureSprite.printf("%5d /div", _xAxisDiv);
  }

  void M5Waveform::_drawXAxisPosLabel(void)
  {
    _figureSprite.setCursor(_waveRegionX + MARGIN, _waveRegionY + _waveRegionHeight + MARGIN);
    _figureSprite.printf("%d", _xAxisPos);
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
        _figureSprite.drawLine(x, y, x + dx * segmentLength / (segmentLength + spaceLength), y + dy * segmentLength / (segmentLength + spaceLength), color);
      }
      x += dx;
      y += dy;
    }
  }
}