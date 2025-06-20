#include "tiledtilelayer.h"

#include <SDL_blendmode.h>
#include <SDL_error.h>
#include <SDL_render.h>

#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/throw_exception.hpp>
#include <fstream>
#include <optional>

#include "VideoManager.h"
#include "captypes.h"
#include "locator.h"
#include "tiledtileset.h"

namespace CapEngine {
using std::filesystem::path;

TiledTileLayer::TiledTileLayer(const jsoncons::json& in_data, std::vector<std::unique_ptr<TiledTileset>>& in_tilesets,
                               int in_tileWidth, int in_tileHeight, int in_mapWidth, int in_mapHeight,
                               std::optional<path> in_path)
    : m_path(std::move(in_path)),
      m_tileWidth(in_tileWidth),
      m_tileHeight(in_tileHeight),
      m_mapWidth(in_mapWidth),
      m_mapHeight(in_mapHeight),
      m_texture(getNullTexturePtr()),
      m_tilesets(in_tilesets)
{
    m_name = in_data["name"].as<std::string>();
    m_height = in_data["height"].as<int>();
    m_width = in_data["width"].as<int>();
    m_x = in_data["x"].as<int>();
    m_y = in_data["y"].as<int>();
    m_visible = in_data["visible"].as<bool>();
    m_data = in_data["data"].as<std::vector<unsigned int>>();

    assert(Locator::videoManager != nullptr);

    // create the texture
    int textureWidth{m_width * m_tileWidth};
    int textureHeight{m_width * m_tileHeight};
    m_texture = Locator::videoManager->createTexturePtr(textureWidth, textureHeight, Colour{0, 0, 0, 0});

    auto& videoManager = Locator::getVideoManager();
    auto* renderer = Locator::videoManager->getRenderer();
    assert(renderer != nullptr);

    // create a transparent tile
    auto transparentTexture = videoManager.createTexturePtr(m_tileWidth, m_tileHeight, Colour{0, 0, 0, 0});

    // populate the texture by reading the data and the tiles from the
    // tilesets Need to read up on global tile ids from
    // https://doc.mapeditor.org/en/stable/reference/global-tile-ids/
    int tileIndex{0};

    for (int y = 0; y < m_mapHeight; ++y) {
        for (int x = 0; x < m_mapWidth; ++x) {
            const GlobalTileInfo tileInfo = getGlobalTileInfo(m_data[tileIndex]);

            // find the tileset
            assert(renderer != nullptr);

            for (auto&& tileset : boost::adaptors::reverse(m_tilesets)) {
                // global tile id is 0 so no tile there, render transparent
                if (tileInfo.globalTileId == 0) {
                    // render to the texture
                    SDL_Rect srcRect{0, 0, m_tileWidth, m_tileHeight};

                    SDL_Rect dstRect{x * m_tileWidth, y * m_tileHeight, m_tileWidth, m_tileHeight};
                    videoManager.drawTexture(m_texture.get(), transparentTexture.get(), dstRect, srcRect);
                }

                else if (tileset->firstGid() <= tileInfo.globalTileId) {
                    int tileId = static_cast<int>(tileInfo.globalTileId) - tileset->firstGid();
                    int xTile = tileId % (tileset->imageWidth() / tileset->tileWidth());
                    int yTile = tileId / (tileset->imageWidth() / tileset->tileWidth());

                    // get the tile from the tileset
                    std::optional<Texture*> texture = tileset->texture();
                    if (texture == std::nullopt) {
                        tileset->loadTexture();
                    }
                    texture = tileset->texture();
                    assert(texture != std::nullopt);

                    // render to the texture
                    SDL_Rect srcRect{xTile * tileset->tileWidth(), yTile * tileset->tileHeight(), tileset->tileWidth(),
                                     tileset->tileHeight()};
                    SDL_Rect dstRect{x * m_tileWidth, y * m_tileHeight, m_tileWidth, m_tileHeight};
                    videoManager.drawTexture(m_texture.get(), texture.value(), dstRect, srcRect);
                }
            }

            tileIndex++;
        }
    }

    Locator::getVideoManager().saveTexture(m_texture.get(), (boost::format("layer_%1%.png") % m_name).str());
}

TiledTileLayer TiledTileLayer::create(std::filesystem::path in_path,
                                      std::vector<std::unique_ptr<TiledTileset>>& in_tilesets, int in_tileWidth,
                                      int in_tileHeight, int in_mapWidth, int in_mapHeight)
{
    std::ifstream f{in_path};
    auto json = jsoncons::json::parse(f);
    return TiledTileLayer(json, in_tilesets, in_tileWidth, in_tileHeight, in_mapWidth, in_mapHeight, in_path);
}

int TiledTileLayer::width() const { return m_width; }
int TiledTileLayer::height() const { return m_height; }
int TiledTileLayer::x() const { return m_x; }
int TiledTileLayer::y() const { return m_y; }
bool TiledTileLayer::visible() const { return m_visible; }
std::vector<unsigned int> const& TiledTileLayer::data() const { return m_data; }

void TiledTileLayer::render(uint32_t in_windowId) const
{
    // do nothing
}

TiledTileLayer::GlobalTileInfo getGlobalTileInfo(unsigned int in_tileId)
{
    const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
    const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
    const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
    const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

    bool flipped_horizontally = (in_tileId & FLIPPED_HORIZONTALLY_FLAG) != 0;
    bool flipped_vertically = (in_tileId & FLIPPED_VERTICALLY_FLAG) != 0;
    bool flipped_diagonally = (in_tileId & FLIPPED_DIAGONALLY_FLAG) != 0;
    bool rotated_hex120 = (in_tileId & ROTATED_HEXAGONAL_120_FLAG) != 0;

    // clear all the flip bits
    in_tileId &=
        ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG | ROTATED_HEXAGONAL_120_FLAG);

    return TiledTileLayer::GlobalTileInfo{flipped_horizontally, flipped_vertically, flipped_diagonally, in_tileId};
}

Texture* TiledTileLayer::texture() { return m_texture.get(); }

}  // namespace CapEngine
