#pragma once

#include <map>
#include <memory>

#include <boost/geometry/arithmetic/arithmetic.hpp>

#include "Geometry.h"
#include "RendererBase.h"

namespace TileEngine {

  class Region : public std::enable_shared_from_this<Region>
  {
  public:
    typedef std::shared_ptr<Region> RegionPtr;
    typedef std::weak_ptr<Region> RegionWeakPtr;
    typedef unsigned long RegionID;

  private:
    typedef struct ChildRegionType {
      Position position;
      RegionPtr region;
    } ChildRegion;
    typedef std::map<RegionID, ChildRegion> RegionsMap;

    typedef std::map<unsigned, RegionPtr> LayersMap;

  public:
    Region();
    Region(RegionWeakPtr parent, RegionID id, unsigned width, unsigned height);
    virtual ~Region();

    RegionID ID() const;
    unsigned Width() const;
    unsigned Height() const;
    Rect GetRect(const Position &relative) const;
    unsigned GetLevelsCount() const;
    RegionPtr AddChild(Position position, unsigned width, unsigned height);
    RegionPtr AddLayer(unsigned level);
    void Render(unsigned level, const Position &position, RendererBase::RendererBasePtr renderer);

    void DrawPrimitive();
    void DrawImage();

  private:
    void RenderSelf(unsigned level, const Position &position, RendererBase::RendererBasePtr renderer);

  private:
    RegionWeakPtr m_parent;
    RegionID m_ID;
    unsigned m_width, m_height;
    RegionsMap m_children;
    LayersMap m_layers;
  };

} // namespace TileEngine
