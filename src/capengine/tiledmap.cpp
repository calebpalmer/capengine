#include "tiledmap.h"

#include <boost/log/trivial.hpp>
#include <boost/throw_exception.hpp>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>

#include "CapEngineException.h"
#include "locator.h"
#include "logging.h"
#include "tiledtilelayer.h"
#include "tiledtileset.h"

namespace CapEngine {

/**
 * \brief Constructs a TiledMap from JSON data.
 * \param in_json The JSON representation of the map.
 * \param in_path Optional path to the map file for resolving relative paths.
 */
TiledMap::TiledMap(const jsoncons::json& in_json, std::optional<std::filesystem::path> in_path)
    : m_path(std::move(in_path)), m_texture(getNullTexturePtr())
{
    this->loadJson(in_json);

    // create the texture
    int textureWidth{m_width * m_tileWidth};
    int textureHeight{m_height * m_tileHeight};
    m_texture = Locator::getVideoManager().createTexturePtr(textureWidth, textureHeight, Colour{0, 0, 0, 0});
}

/**
 * \brief Constructs a TiledMap from a file path.
 * \param in_mapPath Path to the .tmj map file to load.
 */
TiledMap::TiledMap(const std::filesystem::path& in_mapPath) : m_path(in_mapPath), m_texture(getNullTexturePtr())
{
    std::ifstream t(in_mapPath, std::ios::in);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

    std::ifstream f(in_mapPath, std::ios::in);
    auto mapData = jsoncons::json::parse(f);
    this->loadJson(mapData);

    // create the texture
    int textureWidth{m_width * m_tileWidth};
    int textureHeight{m_height * m_tileHeight};
    m_texture = Locator::getVideoManager().createTexturePtr(textureWidth, textureHeight, Colour{0, 0, 0, 0});
}

/**
 * \brief Loads map data from JSON.
 * \param in_json The JSON data to parse.
 */
void TiledMap::loadJson(const jsoncons::json& in_json)
{
    m_tileHeight = in_json["tileheight"].as<int>();
    m_tileWidth = in_json["tilewidth"].as<int>();
    m_width = in_json["width"].as<int>();
    m_height = in_json["height"].as<int>();

    // load tilesets
    m_tilesets.reserve(in_json.count("tilesets"));

    for (auto&& i : in_json["tilesets"].array_range()) {
        if (i.contains("source")) {
            std::string tilesetPath = i["source"].as<std::string>();
            if (m_path) {
                tilesetPath = m_path->parent_path() / tilesetPath;
            }

            if (!i.contains("firstgid")) {
                BOOST_THROW_EXCEPTION(CapEngineException{"tileset missing firstGid property"});
            }
            m_tilesets.push_back(std::make_unique<TiledTileset>(
                TiledTileset::create(i.at("source").as<std::string>(), tilesetPath, i.at("firstgid").as<int>())));
        }
        else {
            if (!i.contains("firstgid")) {
                BOOST_THROW_EXCEPTION(CapEngineException{"tileset missing firstGid property"});
            }
            m_tilesets.push_back(std::make_unique<TiledTileset>("", i, i["firstgid"].as<int>(), this->m_path));
        }
    }

    // load layers
    for (auto&& i : in_json["layers"].array_range()) {
        if (i.at("type").as<std::string>() == "tilelayer") {
            m_layers.emplace_back(i, m_tilesets, m_tileWidth, m_tileHeight, m_width, m_height);
        }

        if (i.at("type").as<std::string>() == "objectgroup") {
            BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Adding objectgroup to map";
            m_objectGroups.emplace_back(i, m_tileWidth * m_width, m_tileHeight * m_height, m_tilesets, m_path);
        }
    }
}

/**
 * \brief Gets the height of individual tiles in pixels.
 * \return The tile height in pixels.
 */
int TiledMap::tileHeight() const { return m_tileHeight; }

/**
 * \brief Gets the width of individual tiles in pixels.
 * \return The tile width in pixels.
 */
int TiledMap::tileWidth() const { return m_tileWidth; }

/**
 * \brief Gets the width of the map in tiles.
 * \return The map width in tiles.
 */
int TiledMap::width() const { return m_width; }

/**
 * \brief Gets the height of the map in tiles.
 * \return The map height in tiles.
 */
int TiledMap::height() const { return m_height; }

/**
 * \brief Gets all tilesets used by this map.
 * \return A vector of unique pointers to TiledTileset objects.
 */
const std::vector<std::unique_ptr<TiledTileset>>& TiledMap::tilesets() const { return m_tilesets; }

/**
 * \brief Gets all tile layers in this map.
 * \return A vector of TiledTileLayer objects.
 */
const std::vector<TiledTileLayer>& TiledMap::layers() const { return m_layers; }

/**
 * \brief Renders all visible layers and objects to the map's texture.
 */
void TiledMap::render()
{
    assert(m_texture != nullptr);

    // render the layers
    std::for_each(m_layers.begin(), m_layers.end(), [this](auto& layer) {
        if (!layer.visible()) {
            return;  // skip invisible layers
        }

        auto* texture = layer.texture();
        assert(texture != nullptr);

        SDL_Rect rect{0, 0, m_width * m_tileWidth, m_height * m_tileHeight};
        Locator::getVideoManager().drawTexture(m_texture.get(), texture, rect, rect);
    });

    // // render objects
    std::for_each(m_objectGroups.begin(), m_objectGroups.end(), [this](auto& objectGroup) {
        if (objectGroup.visible()) {
            objectGroup.render(this->m_texture.get());
        }
    });
}

/**
 * \brief Gets all object groups in this map.
 * \return A vector of TiledObjectGroup objects.
 */
const std::vector<TiledObjectGroup>& TiledMap::objectGroups() const { return m_objectGroups; }

/**
 * \brief Gets the rendered texture of the map.
 * \return A pointer to the map's texture.
 */
Texture* TiledMap::texture() { return m_texture.get(); }

/**
 * \brief Gets a tileset by index.
 * \param index The index of the tileset to retrieve.
 * \return A pointer to the tileset if found, std::nullopt otherwise.
 */
std::optional<TiledTileset const*> TiledMap::tileset(unsigned int index) const
{
    if (index < m_tilesets.size()) {
        return m_tilesets[index].get();
    }

    return std::nullopt;
}

/**
 * \brief Gets an object group by name.
 * \param in_name The name of the object group to find.
 * \return A reference wrapper to the object group if found, std::nullopt otherwise.
 */
std::optional<std::reference_wrapper<const TiledObjectGroup>> TiledMap::objectGroupByName(
    std::string_view in_name) const
{
    auto result = std::find_if(m_objectGroups.begin(), m_objectGroups.end(),
                               [in_name](const TiledObjectGroup& group) { return group.name() == in_name; });
    if (result != m_objectGroups.end()) {
        return std::cref(*result);
    }

    return std::nullopt;
}

}  // namespace CapEngine
