#pragma once

#include <memory>

#include "Geometry.h"

namespace TileEngine {

  class RendererBase;

  class RendererBase
  {
  public:
    typedef std::shared_ptr<RendererBase> RendererBasePtr;

    virtual ~RendererBase() {}

    virtual void RenderBitmap(unsigned level, const Rect &absRect) = 0;
    virtual void RenderPrimitive(unsigned level) = 0;
  };

} // namespace TileEngine