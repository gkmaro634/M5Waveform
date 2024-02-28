#ifndef __M5PLOT_HPP__
#define __M5PLOT_HPP__

#include <M5Unified.h>
#include <M5GFX.h>

namespace m5wf
{
  namespace figure_constants
  {
    static constexpr int MARGIN = 4;
    static constexpr int LABEL_HEIGHT = 16;
    static constexpr int YAXIS_DIV_LABLE_WIDTH = 40;
    static constexpr int YAXIS_DIV_LABLE_HEIGHT = LABEL_HEIGHT * 2;
    static constexpr int YAXIS_POS_LABLE_WIDTH = 40;
    static constexpr int YAXIS_POS_LABLE_HEIGHT = LABEL_HEIGHT;
    static constexpr int XAXIS_DIV_LABLE_WIDTH = 80;
    static constexpr int XAXIS_DIV_LABLE_HEIGHT = LABEL_HEIGHT;
    static constexpr int XAXIS_POS_LABLE_WIDTH = 40;
    static constexpr int XAXIS_POS_LABLE_HEIGHT = LABEL_HEIGHT;
  }

  class M5Plot
  {

  public:
    typedef enum
    {
      X_DIV = 1,
      X_POS,
      Y_DIV,
      Y_POS,
    } EditTarget;

    typedef enum
    {
      NOT_EDIT = 1,
      SELECT,
      EDIT,
    } EditState;

    typedef enum
    {
      LINE = 1,
      MARKER,
      LINE_MARKER
    } PlotType;

    typedef struct 
    {
      float x;
      float y;
    } point_f;

    M5Plot() {}
    M5Plot(M5GFX *display) : _display(display), _canvas(display), _plotSprite(display), _figureSprite(display) { figureCanvas = &_canvas; }
    ~M5Plot()
    {
      _plotSprite.deleteSprite();
      _figureSprite.deleteSprite();
      _canvas.deleteSprite();
    }

    M5Canvas *figureCanvas;

    void init(int32_t width, int32_t height);
    void init(int32_t width, int32_t height, uint8_t xAxisDivCount, uint8_t yAxisDivCount);

    void plot(point_f *points, uint16_t length);
    void plot(point_f *points, uint16_t length, PlotType plotType);
    void updateXAxisDiv(uint16_t value);
    void updateXAxisPos(uint16_t value);
    void updateYAxisDiv(uint16_t value);
    void updateYAxisPos(uint16_t value);

    void setEditState(M5Plot::EditState state);
    void setEditTarget(M5Plot::EditTarget target);

  private:
    M5GFX *_display;
    M5Canvas _canvas;
    M5Canvas _plotSprite;
    M5Canvas _figureSprite;

    M5Plot::EditTarget _editTarget = Y_DIV;
    M5Plot::EditState _editState = NOT_EDIT;

    /// @brief 波形領域全体の幅
    int32_t _figureWidth;
    /// @brief 波形領域全体の高さ
    int32_t _figureHeight;

    uint16_t _plotRegionX = m5wf::figure_constants::MARGIN + m5wf::figure_constants::YAXIS_DIV_LABLE_WIDTH;
    uint16_t _plotRegionY = m5wf::figure_constants::MARGIN;

    uint16_t _plotRegionWidth;
    uint16_t _plotRegionHeight;

    uint8_t _xAxisDivCount = 4; // 0は分割無し, 1で二等分, 2,3,...255
    uint8_t _yAxisDivCount = 3; // 0は分割無し, 1で二等分, 2,3,...255
    uint16_t _xAxisDiv = 40;
    uint16_t _xAxisPos = 0;
    uint16_t _yAxisDiv = 20;
    uint16_t _yAxisPos = 0;

    void _renderFigure(void);
    void _renderPlot(void);

    void _clearCanvas(void);
    void _clearFigure(void);
    void _clearPlot(void);

    void _drawXAxisRulerLine(void);
    void _drawYAxisRulerLine(void);
    void _drawPlotBorder(void);
    void _drawParamEditBorder(void);
    void _drawXAxisDivLabel(void);
    void _drawXAxisPosLabel(void);
    void _drawYAxisDivLabel(void);
    void _drawYAxisPosLabel(void);
    uint8_t _point2px(point_f point, int* x_px, int* y_px);
    void _drawDashedLine(int x0, int y0, int x1, int y1, int segmentLength, int spaceLength, uint16_t color);
  };
}

using M5Plot = m5wf::M5Plot;

#endif