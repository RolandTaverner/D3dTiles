#pragma once

#include <map>
#include <string>

#include "D3d/D3dInterfaces.h"

namespace TileEngine {
namespace D3d {

class TextureCache {
public:
  TextureCache();
  virtual ~TextureCache();

  ID3D11ShaderResourceViewPtr TryGet(const std::string &id);
  bool Set(const std::string &id, ID3D11ShaderResourceViewPtr texture);

private:
  typedef std::map<std::string, ID3D11ShaderResourceViewPtr> CacheType;

  CacheType m_cache;
};

} // namespace D3d
} // namespace TileEngine
