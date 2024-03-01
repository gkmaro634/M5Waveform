#include "TimeSeriesData.hpp"

namespace m5wf
{
    uint8_t TimeSeriesData::init(uint32_t bufferSize)
    {
        if (_handlerQueue != nullptr)
        {
            vQueueDelete(_handlerQueue);
        }

        _handlerQueue = xQueueCreate(bufferSize, sizeof(point_ts));
        if (_handlerQueue == nullptr)
        {
            return return_codes::NG;
        }

        _bufferSize = bufferSize;

        return return_codes::OK;
    }

    uint8_t TimeSeriesData::write(float aData, uint32_t timeoutMs)
    {
        if (_handlerQueue == nullptr)
        {
            return return_codes::NG;
        }

        _prevRsvTimeMs = _currRsvTimeMs;
        auto now = millis();
        float timeDeltaSec;
        if (_prevRsvTimeMs <= now)
        {
            timeDeltaSec = (float)(now - _prevRsvTimeMs) / 1000.0;
        }
        else
        {
            // _currRsvTimeMsがオーバーフローして循環した場合
            timeDeltaSec = (float)(UINT32_MAX - _prevRsvTimeMs + now) / 1000.0;
        }

        point_ts aPoint = {timeDeltaSec, aData};
        auto ret = xQueueSendToBack(_handlerQueue, &aPoint, pdMS_TO_TICKS(timeoutMs));
        if (ret != pdTRUE)
        {
            // Queueが満杯だった場合
            // 前回Write時刻を更新しない or 0リセット
            // 前者を採用する
            return return_codes::NG;
        }
        else
        {
            _currRsvTimeMs = now;
        }

        return return_codes::OK;
    }

    uint8_t TimeSeriesData::write(point_ts aPoint, uint32_t timeoutMs)
    {
        if (_handlerQueue == nullptr)
        {
            return return_codes::NG;
        }

        auto ret = xQueueSendToBack(_handlerQueue, &aPoint, pdMS_TO_TICKS(timeoutMs));
        if (ret != pdTRUE)
        {
            // Queueが満杯だった場合
            // 前回Write時刻を更新しない or 0リセット
            // 前者を採用する
            return return_codes::NG;
        }
        else
        {
            uint32_t rsvTimeMs = _prevRsvTimeMs + aPoint.timeDeltaSecond * 1000;
            _prevRsvTimeMs = _currRsvTimeMs;
            _currRsvTimeMs = rsvTimeMs;
        }

        return return_codes::OK;
    }

    uint8_t TimeSeriesData::read(point_ts *readPoint, uint32_t timeoutMs)
    {
        if (_handlerQueue == nullptr || readPoint == nullptr)
        {
            return return_codes::NG;
        }

        auto result = xQueueReceive(_handlerQueue, readPoint, pdMS_TO_TICKS(timeoutMs));
        if (result != pdPASS)
        {
            return return_codes::NG;
        }

        return return_codes::OK;
    }
}