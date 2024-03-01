#ifndef __M5WAVEFORM2_HPP__
#define __M5WAVEFORM2_HPP__

#include <M5Unified.h>
#include <M5GFX.h>
#include "M5Plot.hpp"
#include "utils/TimeSeriesData.hpp"

static void _drawingTask(void *arg);

namespace m5wf
{
  class M5Waveform : public M5Plot
  {
  public:
    typedef std::function<void()> Callback;

    M5Waveform() : M5Plot() {}
    M5Waveform(M5GFX *display) : M5Plot(display), _tsData() {}
    virtual ~M5Waveform()
    {
      if (_handleDrawingTask != nullptr)
      {
        vTaskDelete(_handleDrawingTask);
      }
      _callback = nullptr;
    }

    uint8_t startDrawing(uint32_t bufferSize);
    uint8_t startDrawing(uint32_t bufferSize, Callback onDrawing);
    uint8_t stopDrawing();

    uint8_t enqueue(float value, uint32_t timeoutMs = 100);
    uint8_t enqueue(point_ts aPoint, uint32_t timeoutMs = 100);

    void job();

  private:
    TimeSeriesData _tsData;
    bool _isDrawing = false;
    Callback _callback = nullptr;

    TaskHandle_t _handleDrawingTask = nullptr;
    point_f _prev;
    point_f _curr;
    bool _hasReachedRightEdge = false;
  };
}

using M5Waveform = m5wf::M5Waveform;

#endif