#ifndef CAPENGINE_TILEDMAP_H
#define CAPENGINE_TILEDMAP_H

#include <filesystem>

namespace CapEngine {
class TiledMap {
public:
  explicit TiledMap(std::string in_mapData);
  explicit TiledMap(const std::filesystem::path &in_mapPath);

  std::string const &getMapData() const;

private:
  std::string m_mapData;
};
} // namespace CapEngine

#endif /* CAPENGINE_TILEDMAP_H */
