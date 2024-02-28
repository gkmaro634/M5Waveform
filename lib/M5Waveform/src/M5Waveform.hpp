#ifndef __M5WAVEFORM_HPP__
#define __M5WAVEFORM_HPP__

#include <M5Unified.h>
#include <M5GFX.h>
#include "M5Plot.hpp"
#include "TimeSeriesData.hpp"

static void _drawingTask(void *arg);

namespace m5wf
{
  class M5Waveform
  {
  public:
    M5Waveform() {}
    M5Waveform(M5Plot *plot) : _plot(plot), _tsData() {}
    ~M5Waveform()
    {
      _tsData.~TimeSeriesData();
      if (_points != nullptr)
      {
        delete[] _points;
        _points = nullptr;
      }

      if (_handleDrawingTask != nullptr)
      {
        vTaskDelete(_handleDrawingTask);
      }
    }

    uint8_t init(uint32_t bufferSize);

    uint8_t startDrawing();
    uint8_t stopDrawing();

    uint8_t enqueue(float value);
    uint8_t enqueue(TimeSeriesData::point_ts aPoint);

    void job();

  private:
    M5Plot* _plot;
    TimeSeriesData _tsData;
    bool _isDrawing = false;

    TaskHandle_t _handleDrawingTask = nullptr;
    // void _drawingTask(void *arg);
    M5Plot::point_f *_points;
    uint32_t _bufferSize = 0;
    uint16_t _pointsLength = 0;
  };
}

using M5Waveform = m5wf::M5Waveform;

#endif