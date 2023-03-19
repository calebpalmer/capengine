#include "tiledtileset.h"
#include "CapEngineException.h"
#include "VideoManager.h"
#include "captypes.h"
#include "locator.h"

#include <boost/throw_exception.hpp>
#include <fstream>

namespace CapEngine {
TiledTileset::TiledTileset(const jsoncons::json &in_data, std::optional<std::filesystem::path> in_path)
    : m_texture(getNullTexturePtr()), m_path(in_path) {
  if (m_path) {
    m_image = m_path->parent_path() / in_data["image"].as<std::string>();
  } else {
    m_image = in_data["image"].as<std::string>();
  }

  m_imageHeight = in_data["imageheight"].as<int>();
  m_imageWidth = in_data["imagewidth"].as<int>();
  m_tileWidth = in_data["tilewidth"].as<int>();
  m_tileHeight = in_data["tileheight"].as<int>();
}

TiledTileset TiledTileset::create(std::filesystem::path in_path) {
  std::ifstream f{in_path};
  auto json = jsoncons::json::parse(f);
  return TiledTileset(json, in_path);
}

const std::string &TiledTileset::image() const { return m_image; }

int TiledTileset::imageWidth() const { return m_imageWidth; }
int TiledTileset::imageHeight() const { return m_imageHeight; }
int TiledTileset::tileWidth() const { return m_tileWidth; }
int TiledTileset::tileHeight() const { return m_tileHeight; }

void TiledTileset::loadTexture() { m_texture = CapEngine::Locator::videoManager->loadImagePtr(m_image); }
Texture &TiledTileset::texture() {
  if (m_texture)
    return *m_texture;
  else
    BOOST_THROW_EXCEPTION(CapEngineException("Texture not loaded. Must call loadTexture() first."));
  ;
}
} // namespace CapEngine
