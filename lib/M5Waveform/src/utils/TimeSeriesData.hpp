#ifndef __TIMESERIESDATA_HPP__
#define __TIMESERIESDATA_HPP__

#include <time.h>
#include <M5Unified.h>
#include "utils/m5wf_common.hpp"

namespace m5wf
{
  class TimeSeriesData
  {

  public:
    TimeSeriesData() {}
    ~TimeSeriesData()
    {
      if (_handlerQueue != NULL)
      {
        vQueueDelete(_handlerQueue);
      }
    }

    uint8_t init(uint32_t bufferSize);

    uint8_t write(float aData, uint32_t timeoutMs = 100);
    uint8_t write(point_ts aPoint, uint32_t timeoutMs = 100);
    uint8_t read(point_ts *readPoint, uint32_t timeoutMs = 100);

  private:
    QueueHandle_t _handlerQueue = nullptr;
    uint32_t _bufferSize = 0;
    uint32_t _prevRsvTimeMs;
    uint32_t _currRsvTimeMs;
  };
}

using TimeSeriesData = m5wf::TimeSeriesData;

#endif