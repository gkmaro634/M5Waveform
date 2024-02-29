#include "M5Waveform.hpp"

using namespace m5wf::figure_constants;

namespace m5wf
{
  uint8_t M5Waveform::startDrawing(uint32_t bufferSize)
  {
    return startDrawing(bufferSize, nullptr);
  }

  uint8_t M5Waveform::startDrawing(uint32_t bufferSize, Callback onDrawing)
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

    // set callback
    _callback = onDrawing;

    // task create
    uint32_t toAllocSize = sizeof(point_f) * _plotRegionWidth + 1024; // 描画対象点のキャッシュ＋処理に必要と思われる領域の予備
    BaseType_t status = xTaskCreatePinnedToCore(_drawingTask, "drawingTask", toAllocSize, this, 1, &_handleDrawingTask, 1);
    configASSERT(status == pdPASS);
    if (status != pdPASS)
    {
      _callback = nullptr;
      return 1;
    }
    
    _isDrawing = true;

    return 0;
  }

  uint8_t M5Waveform::stopDrawing()
  {
    if (_isDrawing == false)
    {
      return 1;
    }
    _isDrawing = false;

    // stop drawingTask
    if (_handleDrawingTask != nullptr)
    {
      vTaskDelete(_handleDrawingTask);
    }

    return 0;
  }

  uint8_t M5Waveform::enqueue(float value)
  {
    return _tsData.write(value);
  }

  uint8_t M5Waveform::enqueue(point_ts aPoint)
  {
    return _tsData.write(aPoint);
  }

  void M5Waveform::job()
  {
    m5wf::point_ts aPoint;
    if (_tsData.read(&aPoint) != 0)
    {
      delay(100); // TODO: 適切な遅延
      return;
    }

    _clearCanvas();

    // 描画更新処理
    _prev = _curr;
    _curr = {_prev.x + aPoint.timeDeltaSecond, aPoint.value};
    int px, py;
    _point2px(_curr, &px, &py);
    float xEnd = _getXAxisEnd();
    _hasReachedRightEdge |= (xEnd < _curr.x);

    if (_hasReachedRightEdge == false)
    {
      // 右端に到達していない場合、左から順番に点を打つ
      _plotSprite.drawCircle(px, py, MARKER_RADIUS, GREEN);
    }
    else
    {
      // 右端に到達したらX差分の分スプライトをシフトし、点を打つ
      float xStart = _getXAxisStart();
      float dx = _plotRegionWidth / (xEnd - xStart);
      int_fast16_t xScroll = -dx * aPoint.timeDeltaSecond;
      _plotSprite.scroll(xScroll, 0);

      _plotSprite.drawCircle((int)_plotRegionWidth - MARKER_RADIUS, py, MARKER_RADIUS, GREEN);
    }

    _renderFigure();
    _renderPlot();

    // 描画更新完了コールバック関数を呼ぶ
    if (_callback != nullptr)
    {
      _callback();
    }

    // TODO: Y軸レンジ変更での再描画に備えて描画済み点群をキャッシュするべきか <= 点が膨大になりうる。しかしながらキャッシュしないと突発的な大きな信号の表示に耐えられない。
    // Task生成時に確保したメモリ以下に収まる保証が必要である。最大点数は描画幅までで済むはずなので、その方向で検討する。
    // 同一X画素位置に複数点が存在する場合のケアも必要。最大最小を保持するか逐次平均か。前者のほうがデータを誤解なく表現できる。
  }
}

static void _drawingTask(void *arg)
{
  // M5Waveformオブジェクトへのポインタにキャスト
  auto *waveform = static_cast<m5wf::M5Waveform *>(arg);

  while (true)
  {
    // FreeRTOSのTask内ではインスタンスのメンバにアクセスできないので、仕方なく専用のpublicメソッドを設けて呼び出すこととする。
    waveform->job();
  }
}