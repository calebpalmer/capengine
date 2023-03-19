#include "tiledtilelayer.h"
#include "VideoManager.h"
#include "locator.h"

#include <fstream>

namespace CapEngine {
using std::filesystem::path;

TiledTileLayer::TiledTileLayer(
    const jsoncons::json &in_data,
    std::weak_ptr<std::vector<TiledTileset>> in_tilesets, int in_tileWidth,
    int in_tileHeight, std::optional<path> in_path)
    : m_path(in_path),
      m_tileWidth(in_tileWidth),
      m_tileHeight(in_tileHeight),
      m_texture(getNullTexturePtr()),
      m_tilesets(in_tilesets) {
  m_height = in_data["height"].as<int>();
  m_width = in_data["width"].as<int>();
  m_x = in_data["x"].as<int>();
  m_y = in_data["y"].as<int>();
  m_visible = in_data["visible"].as<bool>();
  m_data = in_data["data"].as<std::vector<int>>();

  assert(Locator::videoManager != nullptr);

  // create the texture
  int textureWidth{m_width * m_tileWidth};
  int textureHeight{m_width * m_tileHeight};
  m_texture =
      Locator::videoManager->createTexturePtr(textureWidth, textureHeight);

  // TODO populate the texture by reading the data and the tiles from the
  // tilesets Need to read up on global tile ids from
  // https://doc.mapeditor.org/en/stable/reference/global-tile-ids/
}

TiledTileLayer TiledTileLayer::create(
    std::filesystem::path in_path,
    std::weak_ptr<std::vector<TiledTileset>> in_tilesets, int in_tileWidth,
    int in_tileHeight) {
  std::ifstream f{in_path};
  auto json = jsoncons::json::parse(f);
  return TiledTileLayer(json, std::move(in_tilesets), in_tileWidth,
                        in_tileHeight, in_path);
}

int TiledTileLayer::width() const { return m_width; }
int TiledTileLayer::height() const { return m_height; }
int TiledTileLayer::x() const { return m_x; }
int TiledTileLayer::y() const { return m_y; }
bool TiledTileLayer::visible() const { return m_visible; }
const std::vector<int> TiledTileLayer::data() const { return m_data; }

TiledTileLayer::GlobalTileInfo getGlobalTileInfo(unsigned int in_data) {
  const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
  const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
  const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
  const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

  bool flipped_horizontally = (in_data & FLIPPED_HORIZONTALLY_FLAG);
  bool flipped_vertically = (in_data & FLIPPED_VERTICALLY_FLAG);
  bool flipped_diagonally = (in_data & FLIPPED_DIAGONALLY_FLAG);
  bool rotated_hex120 = (in_data & ROTATED_HEXAGONAL_120_FLAG);

  return TiledTileLayer::GlobalTileInfo{flipped_horizontally,
                                        flipped_vertically, flipped_diagonally};
}

} // namespace CapEngine
