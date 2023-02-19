#include "tiledmap.h"

#include <fstream>
#include <sstream>

namespace CapEngine {

TiledMap::TiledMap(std::string in_mapData) : m_mapData(std::move(in_mapData)) {}

TiledMap::TiledMap(const std::filesystem::path &in_mapPath) {
  std::ifstream stream(in_mapPath, std::ios::in);

  std::ostringstream str;
  std::string data;
  while (std::getline(stream, data)) {
    str << data;
  }

  m_mapData = str.str();
}

std::string const &TiledMap::getMapData() const { return m_mapData; }
} // namespace CapEngine
