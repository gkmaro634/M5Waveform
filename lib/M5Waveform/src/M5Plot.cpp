#include "M5Plot.hpp"

using namespace m5wf::figure_constants;

namespace m5wf
{
  void M5Plot::init(int32_t width, int32_t height, uint8_t xAxisDivCount, uint8_t yAxisDivCount)
  {
    _xAxisDivCount = xAxisDivCount;
    _yAxisDivCount = yAxisDivCount;

    M5Plot::init(width, height);
  }

  void M5Plot::init(int32_t width, int32_t height)
  {
    _figureWidth = width;
    _figureHeight = height;

    _plotRegionWidth = _figureWidth - MARGIN - MARGIN - YAXIS_DIV_LABLE_WIDTH;
    _plotRegionHeight = _figureHeight - MARGIN - MARGIN - XAXIS_DIV_LABLE_HEIGHT;

    _canvas.createSprite(_figureWidth, _figureHeight);
    _canvas.setColorDepth(8);
    _canvas.setPivot(_figureWidth / 2, _figureHeight / 2);

    _plotSprite.createSprite(_plotRegionWidth, _plotRegionHeight);
    _plotSprite.setColorDepth(8);

    _figureSprite.createSprite(_figureWidth, _figureHeight);
    _figureSprite.setColorDepth(8);

    // 分割点線
    _drawXAxisRulerLine();
    _drawYAxisRulerLine();

    // 枠
    _drawPlotBorder();

    // 選択枠
    _drawParamEditBorder();

    // 軸ラベル
    _drawXAxisDivLabel();
    _drawXAxisPosLabel();
    _drawYAxisDivLabel();
    _drawYAxisPosLabel();

    _renderFigure();
  }

  void M5Plot::plot(point_f *points, uint16_t length, PlotType plotType)
  {
    _clearCanvas();
    _clearPlot();

    // point
    if (plotType == LINE_MARKER || plotType == MARKER)
    {
      int x_pt, y_pt;
      for (int i = 0; i < length; i++)
      {
        auto p = points[i];
        if (_point2px(p, &x_pt, &y_pt) == 0)
        {
          _plotSprite.fillCircle(x_pt, y_pt, 2, GREEN);
        }
      }
    }

    // line
    if (plotType == LINE_MARKER || plotType == LINE)
    {
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

        _plotSprite.drawLine(x1_pt, y1_pt, x2_pt, y2_pt, GREEN);
      }
    }

    _renderFigure();
    _renderPlot();
  }

  void M5Plot::plot(point_f *points, uint16_t length)
  {
    plot(points, length, LINE_MARKER);
  }

  void M5Plot::updateXAxisDiv(uint16_t value)
  {
    _xAxisDiv = value;
    _drawXAxisDivLabel();

    _clearCanvas();
    _renderFigure();
    _renderPlot();
  }

  void M5Plot::updateXAxisPos(uint16_t value)
  {
    _xAxisPos = value;
    _drawXAxisPosLabel();
    _renderFigure();
  }

  void M5Plot::updateYAxisDiv(uint16_t value)
  {
    _yAxisDiv = value;
    _drawYAxisDivLabel();
    _renderFigure();
  }

  void M5Plot::updateYAxisPos(uint16_t value)
  {
    _yAxisPos = value;
    _drawYAxisPosLabel();
    _renderFigure();
  }

  void M5Plot::setEditState(EditState state)
  {
    _clearCanvas();

    _editState = state;
    _drawParamEditBorder();

    _renderFigure();
    _renderPlot();
  }

  void M5Plot::setEditTarget(EditTarget target)
  {
    _clearCanvas();

    _editTarget = target;
    _drawParamEditBorder();

    _renderFigure();
    _renderPlot();
  }

  void M5Plot::_renderFigure(void)
  {
    _figureSprite.pushSprite(&_canvas, 0, 0, BLACK);
  }

  void M5Plot::_renderPlot(void)
  {
    _plotSprite.pushSprite(&_canvas, (int32_t)_plotRegionX, (int32_t)_plotRegionY, BLACK);
  }

  void M5Plot::_clearCanvas(void)
  {
    _canvas.fillScreen(BLACK);
  }

  void M5Plot::_clearPlot(void)
  {
    _plotSprite.fillScreen(BLACK);
  }

  void M5Plot::_clearFigure(void)
  {
    _figureSprite.fillScreen(BLACK);
  }

  uint8_t M5Plot::_point2px(point_f point, int *x_px, int *y_px)
  {
    float xStart = _getXAxisStart();
    float xEnd = _getXAxisEnd();
    float yStart = _getYAxisStart();
    float yEnd = _getYAxisEnd();
    uint8_t ret = 0;

    if (point.x < xStart || xEnd < point.x || point.y < yStart || yEnd < point.y)
    {
      ret = 1;
    }

    float dx = (float)_plotRegionWidth / (xEnd - xStart);
    float dy = (float)_plotRegionHeight / (yEnd - yStart);
    *x_px = (int)((point.x - xStart) * dx);
    *y_px = (int)(_plotRegionHeight - (point.y - yStart) * dy);

    return ret;
  }

  void M5Plot::_drawXAxisRulerLine(void)
  {
    float step = (float)_plotRegionWidth / (float)(_xAxisDivCount + 1);
    for (int i = 0; i < _xAxisDivCount; i++)
    {
      float xPos = step * (i + 1) + _plotRegionX;
      _drawDashedLine((int)xPos, _plotRegionY, (int)xPos, _plotRegionY + _plotRegionHeight, 6, 2, DARKGREEN);
    }
  }

  void M5Plot::_drawYAxisRulerLine(void)
  {
    float step = (float)_plotRegionHeight / (float)(_yAxisDivCount + 1);
    for (int i = 0; i < _yAxisDivCount; i++)
    {
      float yPos = step * (i + 1) + _plotRegionY;
      _drawDashedLine(_plotRegionX, (int)yPos, _plotRegionX + _plotRegionWidth, (int)yPos, 6, 2, DARKGREEN);
    }
  }

  void M5Plot::_drawPlotBorder(void)
  {
    _figureSprite.drawRect(
        _plotRegionX,
        _plotRegionY,
        _plotRegionWidth,
        _plotRegionHeight,
        DARKGREEN);
  }

  void M5Plot::_drawParamEditBorder(void)
  {
    _figureSprite.drawRect(MARGIN, _plotRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, BLACK);
    _figureSprite.drawRect(MARGIN, _plotRegionY + _plotRegionHeight - YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, BLACK);
    _figureSprite.drawRect(_plotRegionX + _plotRegionWidth - XAXIS_DIV_LABLE_WIDTH, _plotRegionY + _plotRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, BLACK);
    _figureSprite.drawRect(_plotRegionX, _plotRegionY + _plotRegionHeight, XAXIS_POS_LABLE_WIDTH, XAXIS_POS_LABLE_HEIGHT, BLACK);

    if (_editState == NOT_EDIT)
    {
      return;
    }

    int borderColor;
    switch (_editState)
    {
    case SELECT:
      borderColor = DARKGREEN;
      break;
    case EDIT:
      borderColor = RED;
      break;
    default:
      borderColor = BLACK;
      break;
    }

    switch (_editTarget)
    {
    case Y_DIV:
      _figureSprite.drawRect(MARGIN, _plotRegionY, YAXIS_DIV_LABLE_WIDTH, YAXIS_DIV_LABLE_HEIGHT, borderColor);
      break;
    case Y_POS:
      _figureSprite.drawRect(MARGIN, _plotRegionY + _plotRegionHeight - YAXIS_POS_LABLE_HEIGHT, YAXIS_POS_LABLE_WIDTH, YAXIS_POS_LABLE_HEIGHT, borderColor);
      break;
    case X_DIV:
      _figureSprite.drawRect(_plotRegionX + _plotRegionWidth - XAXIS_DIV_LABLE_WIDTH, _plotRegionY + _plotRegionHeight, XAXIS_DIV_LABLE_WIDTH, XAXIS_DIV_LABLE_HEIGHT, borderColor);
      break;
    case X_POS:
      _figureSprite.drawRect(_plotRegionX, _plotRegionY + _plotRegionHeight, XAXIS_POS_LABLE_WIDTH, XAXIS_POS_LABLE_HEIGHT, borderColor);
      break;
    default:
      break;
    }
  }

  void M5Plot::_drawYAxisDivLabel(void)
  {
    _figureSprite.setCursor(MARGIN + MARGIN, _plotRegionY + MARGIN);
    _figureSprite.printf("%d", _yAxisDiv);
    _figureSprite.setCursor(MARGIN + MARGIN, _plotRegionY + MARGIN + LABEL_HEIGHT);
    _figureSprite.printf("/div");
  }

  void M5Plot::_drawYAxisPosLabel(void)
  {
    _figureSprite.setCursor(MARGIN + MARGIN, _plotRegionY + _plotRegionHeight - YAXIS_POS_LABLE_HEIGHT + MARGIN);
    _figureSprite.printf("%5d", _yAxisPos);
  }

  void M5Plot::_drawXAxisDivLabel(void)
  {
    _figureSprite.setCursor(_plotRegionX + _plotRegionWidth - XAXIS_DIV_LABLE_WIDTH + MARGIN, _plotRegionY + _plotRegionHeight + MARGIN);
    _figureSprite.printf("%5d /div", _xAxisDiv);
  }

  void M5Plot::_drawXAxisPosLabel(void)
  {
    _figureSprite.setCursor(_plotRegionX + MARGIN, _plotRegionY + _plotRegionHeight + MARGIN);
    _figureSprite.printf("%d", _xAxisPos);
  }

  float M5Plot::_getXAxisStart()
  {
    return (float)_xAxisPos;
  }

  float M5Plot::_getYAxisStart()
  {
    return (float)_yAxisPos;
  }

  float M5Plot::_getXAxisEnd()
  {
    return (float)_xAxisPos + (float)_xAxisDiv * ((float)_xAxisDivCount + (float)1);
  }

  float M5Plot::_getYAxisEnd()
  {
    return (float)_yAxisPos + (float)_yAxisDiv * ((float)_yAxisDivCount + (float)1);
  }
  
  void M5Plot::_drawDashedLine(int x0, int y0, int x1, int y1, int segmentLength, int spaceLength, uint16_t color)
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
      {
        // セグメントを描画
        _figureSprite.drawLine(x, y, x + dx * segmentLength / (segmentLength + spaceLength), y + dy * segmentLength / (segmentLength + spaceLength), color);
      }
      x += dx;
      y += dy;
    }
  }

}