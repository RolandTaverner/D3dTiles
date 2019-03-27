#pragma once

#include "Bitmap.h"
#include "D3d/D3dInterfaces.h"

namespace TileEngine::D3d::Utils {

ID3D11ShaderResourceViewPtr CreateD3dTexture(const TileEngine::Bitmap &s);

} // namespace TileEngine::D3d::Utils


