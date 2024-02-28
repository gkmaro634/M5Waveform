#ifndef __M5WAVEFORM2_HPP__
#define __M5WAVEFORM2_HPP__

#include <M5Unified.h>
#include <M5GFX.h>
#include "M5Plot.hpp"
#include "TimeSeriesData.hpp"

static void _drawingTask(void *arg);

namespace m5wf
{
  class M5Waveform2 : public M5Plot
  {
  public:
    M5Waveform2() : M5Plot() {}
    M5Waveform2(M5GFX *display) : M5Plot(display), _tsData() {}
    virtual ~M5Waveform2()
    {
    }

    uint8_t startDrawing(uint32_t bufferSize);
    uint8_t stopDrawing();

    uint8_t enqueue(float value);
    uint8_t enqueue(point_ts aPoint);

    void job();

  private:
    TimeSeriesData _tsData;
    bool _isDrawing = false;

    TaskHandle_t _handleDrawingTask = nullptr;
    point_f _prev;
    point_f _curr;
    bool _hasReachedRightEdge = false;
  };
}

using M5Waveform2 = m5wf::M5Waveform2;

#endif