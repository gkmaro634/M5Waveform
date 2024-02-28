#include "M5Waveform2.hpp"

namespace m5wf
{
  uint8_t M5Waveform2::startDrawing(uint32_t bufferSize)
  {
    if (_isDrawing == true)
    {
      return 1;
    }

    // _bufferSize = 0;
    if (_tsData.init(bufferSize) != 0)
    {
      return 1;
    }

    // if (_points != nullptr)
    // {
    //   delete[] _points;
    //   _points = nullptr;
    // }

    // _points = new point_f[bufferSize];
    // if (_points == nullptr)
    // {
    //   return 1;
    // }
    // _bufferSize = bufferSize;

    if (_handleDrawingTask != nullptr)
    {
      vTaskDelete(_handleDrawingTask);
    }

    // task create
    uint32_t toAllocSize = sizeof(point_f) * bufferSize + 1024;
    BaseType_t status = xTaskCreatePinnedToCore(_drawingTask, "drawingTask", toAllocSize, this, 1, &_handleDrawingTask, 1);
    configASSERT(status == pdPASS);
    if (status != pdPASS)
    {
      return 1;
    }
    // else
    // {
    //   vTaskSuspend(_handleDrawingTask);
    // }

    _isDrawing = true;

    // // launch drawingTask
    // vTaskResume(_handleDrawingTask);

    return 0;
  }

  uint8_t M5Waveform2::stopDrawing()
  {
    if (_isDrawing == false)
    {
      return 1;
    }
    _isDrawing = false;

    // stop drawingTask
    // vTaskSuspend(_handleDrawingTask);
    if (_handleDrawingTask != nullptr)
    {
      vTaskDelete(_handleDrawingTask);
    }

    return 0;
  }

  uint8_t M5Waveform2::enqueue(float value)
  {
    return _tsData.write(value);
  }

  uint8_t M5Waveform2::enqueue(point_ts aPoint)
  {
    return _tsData.write(aPoint);
  }

  void M5Waveform2::job()
  {
    m5wf::point_ts aPoint;
    if (_tsData.read(&aPoint) != 0)
    {
      delay(100); // TODO: 適切な遅延
    }

    // 描画更新処理
    // 右端に到達していない場合、左から順番に点を打つ
    // 右端に到達したらX差分の分スプライトをシフトし、点を打つ
    // Y軸レンジ変更での再描画に備えて描画済み点群をキャッシュするべきか <= 点が膨大になりうる。しかしながらキャッシュしないと突発的な大きな信号の表示に耐えられない。

    // 自身で保持する配列に割り当てる
    // 後ろに詰める
    // for (int i = _bufferSize - 1; i >= 1; i--)
    // {
    //   _points[i] = _points[i - 1];
    // }
    // _points[0] = {aPoint.timeDeltaSecond, aPoint.value};

    // if (_pointsLength < _bufferSize)
    // {
    //   _pointsLength++;
    // }

    // // Plotにわたす
    // _plot->plot(_points, _pointsLength);
  }
}

static void _drawingTask(void *arg)
{
  // M5Waveformオブジェクトへのポインタにキャスト
  auto *waveform = static_cast<m5wf::M5Waveform2 *>(arg);

  while (true)
  {
    // FreeRTOSのTask内ではインスタンスのメンバにアクセスできないので、仕方なく専用のpublicメソッドを設けて呼び出すこととする。
    waveform->job();
 }
}