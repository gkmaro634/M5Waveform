#ifndef __M5WAVEFORM_HPP__
#define __M5WAVEFORM_HPP__

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
  }

  class M5Waveform
  {
  public:
    typedef enum
    {
      Y_DIV = 1,
      Y_POS,
      X_DIV
    } EditSelection;

    M5Waveform();
    M5Waveform(M5Canvas* canvas);
    ~M5Waveform();

    M5Canvas* figureCanvas;

    void init(int32_t width, int32_t height);

  private:
    /// @brief 波形領域全体の幅
    int32_t _figureWidth;
    /// @brief 波形領域全体の高さ
    int32_t _figureHeight;

    EditSelection _selection = Y_DIV;
    uint8_t _yAxisDivCount = 1; // 0は分割無し, 1で二等分, 2,3,...255
    uint8_t _xAxisDivCount = 4; // 0は分割無し, 1で二等分, 2,3,...255
    uint16_t _yAxisDiv = 20;
    uint16_t _yAxisPos = 100;
    uint16_t _xAxisDiv = 50;

    void _drawDashedLine(int x0, int y0, int x1, int y1, int segmentLength, int spaceLength, uint16_t color);
  };
}

#endif