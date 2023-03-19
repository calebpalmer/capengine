#include "tiledmap.h"
#include "tiledtilelayer.h"
#include "tiledtileset.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace CapEngine {

TiledMap::TiledMap(const jsoncons::json &in_json, std::optional<std::filesystem::path> in_path)
    : m_path(in_path), m_tilesets(std::make_shared<std::vector<TiledTileset>>()) {
  this->loadJson(in_json);
}

TiledMap::TiledMap(const std::filesystem::path &in_mapPath)
    : m_path(in_mapPath), m_tilesets(std::make_shared<std::vector<TiledTileset>>()) {
  std::ifstream f(in_mapPath, std::ios::in);
  auto mapData = jsoncons::json::parse(f);
  this->loadJson(mapData);
}

void TiledMap::loadJson(const jsoncons::json &in_json) {
  m_tileHeight = in_json["tileheight"].as<int>();
  m_tileWidth = in_json["tilewidth"].as<int>();
  m_width = in_json["width"].as<int>();
  m_height = in_json["height"].as<int>();

  // load tilesets
  assert(m_tilesets);
  m_tilesets->reserve(in_json.count("tilesets"));
  for (auto &&i : in_json["tilesets"].array_range()) {
    std::string tilesetPath = i["source"].as<std::string>();
    if (m_path)
      tilesetPath = m_path->parent_path() / tilesetPath;

    m_tilesets->push_back(TiledTileset::create(tilesetPath));
  }

  for (auto &&i : in_json["layers"].array_range()) {
    if (i.at("type").as<std::string>() == "tilelayer") {
      m_layers.push_back(TiledTileLayer{i, m_tilesets, m_tileWidth, m_tileHeight});
    }
  }
}

int TiledMap::tileHeight() const { return m_tileHeight; }
int TiledMap::tileWidth() const { return m_tileWidth; }
int TiledMap::width() const { return m_width; }
int TiledMap::height() const { return m_height; }

const std::vector<TiledTileset> &TiledMap::tilesets() const { return *m_tilesets; }
const std::vector<TiledTileLayer> &TiledMap::layers() const { return m_layers; }
} // namespace CapEngine
