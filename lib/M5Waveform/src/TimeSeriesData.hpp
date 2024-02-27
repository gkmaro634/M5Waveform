#ifndef __TIMESERIESDATA_HPP__
#define __TIMESERIESDATA_HPP__

#include <mutex>
#include <time.h>
#include <M5Unified.h>

namespace m5wf
{
  class TimeSeriesData
  {

  public:
    typedef struct
    {
      float timeDeltaSecond;
      float value;
    } point_ts;

    TimeSeriesData() {}
    ~TimeSeriesData()
    {
      if (_buffer != nullptr)
      {
        delete[] _buffer;
        _buffer = nullptr;
      }
    }

    uint8_t init(uint32_t bufferSize);

    uint8_t write(float aData);
    uint8_t write(point_ts aPoint);
    uint8_t read(point_ts* readPoint);

  private:
    std::mutex _mutex;
    uint32_t _bufferSize = 0;
    uint32_t _wPtr = 0;
    uint32_t _rPtr = 0;
    point_ts *_buffer;
    uint32_t _prevRsvTimeMs;
    uint32_t _currRsvTimeMs;

    void _addPointToBuffer(const point_ts &point);
  };
}

using TimeSeriesData = m5wf::TimeSeriesData;

#endif