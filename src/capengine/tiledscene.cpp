#include "tiledscene.h"

namespace CapEngine {
TiledScene::TiledScene(jsoncons::json json) : m_json(std::move(json)) {}
} // namespace CapEngine
