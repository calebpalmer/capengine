#ifndef CAPENGINE_TILEDSCENE_H
#define CAPENGINE_TILEDSCENE_H

#include <jsoncons/json.hpp>

#include <filesystem>
#include <functional>

namespace CapEngine {
class TiledScene {
public:
  explicit TiledScene(jsoncons::json json);

  void update(double in_ms);
  void render(uint32_t in_windowId);
  void setEndSceneCB(std::function<void()> in_endSceneCB);

private:
  jsoncons::json m_json;
};
} // namespace CapEngine

#endif /* CAPENGINE_TILEDSCENE_H */
