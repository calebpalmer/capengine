#include "tiledtileset.h"

#include <boost/throw_exception.hpp>
#include <fstream>
#include <optional>
#include <utility>

#include "CapEngineException.h"
#include "VideoManager.h"
#include "captypes.h"
#include "collision.h"
#include "locator.h"

namespace CapEngine
{
TiledTileset::TiledTileset(std::string in_name, const jsoncons::json &in_data,
                           int in_firstGid,
                           std::optional<std::filesystem::path> in_path)
    : m_name(std::move(in_name)),
      m_texture(getNullTexturePtr()),
      m_path(std::move(in_path)),
      m_firstGid(in_firstGid)
{
    if (m_path) {
        m_image = m_path->parent_path() / in_data["image"].as<std::string>();
    } else {
        m_image = in_data["image"].as<std::string>();
    }

    m_imageHeight = in_data["imageheight"].as<int>();
    m_imageWidth = in_data["imagewidth"].as<int>();
    m_tileWidth = in_data["tilewidth"].as<int>();
    m_tileHeight = in_data["tileheight"].as<int>();

    if (in_data.contains("tileoffset")) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("\"tileoffset\" property not yet supported."));
    }

    if (in_data.contains("grid")) {
        BOOST_THROW_EXCEPTION(
            CapEngineException("\"grid\" property not yet supported."));
    }
}

TiledTileset TiledTileset::create(std::string in_name,
                                  std::filesystem::path in_path,
                                  int in_firstGid)
{
    std::ifstream f{in_path};
    auto json = jsoncons::json::parse(f);
    return TiledTileset(std::move(in_name), json, in_firstGid, in_path);
}

const std::string &TiledTileset::image() const { return m_image; }

const std::string &TiledTileset::name() const { return m_name; }
int TiledTileset::firstGid() const { return m_firstGid; }
int TiledTileset::imageWidth() const { return m_imageWidth; }
int TiledTileset::imageHeight() const { return m_imageHeight; }
int TiledTileset::tileWidth() const { return m_tileWidth; }
int TiledTileset::tileHeight() const { return m_tileHeight; }

void TiledTileset::loadTexture()
{
    m_texture = CapEngine::Locator::videoManager->loadImagePtr(m_image);
}

std::optional<Texture *> TiledTileset::texture()
{
    if (m_texture) {
        return m_texture.get();
    }
    return std::nullopt;
}

Rectangle TiledTileset::tileRect(int in_tileId) const
{
    int tileId = in_tileId - m_firstGid;
    int xTile = in_tileId % (this->imageWidth() / this->tileWidth());
    int yTile = in_tileId / (this->imageWidth() / this->tileWidth());

    return Rectangle{xTile * this->tileWidth(), yTile * this->tileHeight(),
                     this->tileWidth(), this->tileHeight()};
}

}  // namespace CapEngine
