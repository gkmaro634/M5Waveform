#ifndef __M5WF_COMMON_HPP__
#define __M5WF_COMMON_HPP__

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
    static constexpr int MARKER_RADIUS = 2;
  }

  namespace return_codes
  {
    static constexpr uint8_t OK = 0;
    static constexpr uint8_t NG = 1;
  }

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

  typedef struct
  {
    float timeDeltaSecond;
    float value;
  } point_ts;

}

#endif