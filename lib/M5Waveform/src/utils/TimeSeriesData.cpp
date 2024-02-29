#include "TimeSeriesData.hpp"

namespace m5wf
{
    uint8_t TimeSeriesData::init(uint32_t bufferSize)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_buffer != nullptr)
        {
            delete[] _buffer;
            _buffer = nullptr;
        }

        _bufferSize = bufferSize;
        _buffer = new point_ts[bufferSize];
        if (_buffer == nullptr)
        {
            _bufferSize = 0;
            return 1;
        }

        return 0;
    }

    uint8_t TimeSeriesData::write(float aData)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_buffer == nullptr)
        {
            return 1;
        }

        _prevRsvTimeMs = _currRsvTimeMs;
        _currRsvTimeMs = millis();
        float timeDeltaSec;
        if (_prevRsvTimeMs <= _currRsvTimeMs)
        {
            timeDeltaSec = (float)(_currRsvTimeMs - _prevRsvTimeMs) / 1000.0;
        }
        else
        {
            timeDeltaSec = (float)(UINT32_MAX - _prevRsvTimeMs + _currRsvTimeMs) / 1000.0;
        }

        point_ts aPoint = {timeDeltaSec, aData};
        _addPointToBuffer(aPoint);

        return 0;
    }

    uint8_t TimeSeriesData::write(point_ts aPoint)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_buffer == nullptr)
        {
            return 1;
        }

        uint32_t rsvTimeMs = _prevRsvTimeMs + aPoint.timeDeltaSecond * 1000;
        _prevRsvTimeMs = _currRsvTimeMs;
        _currRsvTimeMs = rsvTimeMs;

        _addPointToBuffer(aPoint);

        return 0;
    }

    uint8_t TimeSeriesData::read(point_ts *readPoint)
    {
        std::lock_guard<std::mutex> lock(_mutex);

        if (_buffer == nullptr || readPoint == nullptr)
        {
            return 1;
        }

        if (_rPtr == _wPtr)
        {
            return 2;
        }

        *readPoint = _buffer[_rPtr];

        _rPtr++;
        if (_rPtr >= _bufferSize)
        {
            _rPtr = 0;
        }

        return 0;
    }

    void TimeSeriesData::_addPointToBuffer(const point_ts &point)
    {
        _buffer[_wPtr++] = point;
        if (_wPtr >= _bufferSize)
        {
            _wPtr = 0;
        }
    }
}