#ifndef CAPENGINE_TILEDMAP_H
#define CAPENGINE_TILEDMAP_H

#include <filesystem>
#include <functional>
#include <jsoncons/json.hpp>

#include "tiledobjectgroup.h"
#include "tiledtilelayer.h"
#include "tiledtileset.h"

namespace CapEngine {
//! Tiled map representation
/**
 * \brief Represents a complete Tiled map with layers, objects, and tilesets.
 * 
 * This class loads and manages Tiled maps created by the Tiled map editor.
 * It supports tile layers, object groups, and multiple tilesets. The map
 * can be loaded from JSON data or directly from a .tmj file.
 */
class TiledMap final {
   public:
    explicit TiledMap(const jsoncons::json& in_json, std::optional<std::filesystem::path> in_path = std::nullopt);
    explicit TiledMap(const std::filesystem::path& in_mapPath);

    [[nodiscard]] int tileHeight() const;
    [[nodiscard]] int tileWidth() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] const std::vector<std::unique_ptr<TiledTileset>>& tilesets() const;
    [[nodiscard]] const std::vector<TiledTileLayer>& layers() const;
    [[nodiscard]] const std::vector<TiledObjectGroup>& objectGroups() const;

    [[nodiscard]] std::optional<TiledTileset const*> tileset(unsigned int index) const;
    [[nodiscard]] std::optional<std::reference_wrapper<const TiledObjectGroup>> objectGroupByName(
        std::string_view in_name) const;

    void render();
    Texture* texture();

   private:
    void loadJson(const jsoncons::json& in_json);

    std::optional<std::filesystem::path> m_path;          //!< Optional path to the map file
    TexturePtr m_texture;                                 //!< Rendered texture of the entire map
    int m_tileHeight{0};                                  //!< Height of individual tiles in pixels
    int m_tileWidth{0};                                   //!< Width of individual tiles in pixels
    int m_width{0};                                       //!< Width of the map in tiles
    int m_height{0};                                      //!< Height of the map in tiles
    std::vector<std::unique_ptr<TiledTileset>> m_tilesets; //!< All tilesets used by this map
    std::vector<TiledTileLayer> m_layers;                 //!< All tile layers in the map
    std::vector<TiledObjectGroup> m_objectGroups;         //!< All object groups in the map
};
}  // namespace CapEngine

#endif /* CAPENGINE_TILEDMAP_H */
