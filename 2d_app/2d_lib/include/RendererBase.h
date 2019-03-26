#pragma once

#include <memory>

#include "Bitmap.h"
#include "Geometry.h"

namespace TileEngine {

  class RendererBase;

  class RendererBase
  {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;

    virtual ~RendererBase() {}

    virtual void RenderBitmap(unsigned level, const Rect &absRect, Bitmap::BitmapPtr s) = 0;
    virtual void RenderPrimitive(unsigned level) = 0;
  };

} // namespace TileEngine