#ifndef CAPENGINE_TILEDMAP_H
#define CAPENGINE_TILEDMAP_H

#include "tiledtilelayer.h"
#include "tiledtileset.h"

#include <filesystem>
#include <jsoncons/json.hpp>

namespace CapEngine {
class TiledMap final {
public:
  explicit TiledMap(const jsoncons::json &in_json, std::optional<std::filesystem::path> in_path = std::nullopt);
  explicit TiledMap(const std::filesystem::path &in_mapPath);

  int tileHeight() const;
  int tileWidth() const;
  int width() const;
  int height() const;
  const std::vector<TiledTileset> &tilesets() const;
  const std::vector<TiledTileLayer> &layers() const;

private:
  void loadJson(const jsoncons::json &in_json);

  std::optional<std::filesystem::path> m_path;
  int m_tileHeight{0};
  int m_tileWidth{0};
  int m_width{0};
  int m_height{0};
  std::shared_ptr<std::vector<TiledTileset>> m_tilesets;
  std::vector<TiledTileLayer> m_layers;
};
} // namespace CapEngine

#endif /* CAPENGINE_TILEDMAP_H */
