#ifndef CAPENGINE_TILEDTILELAYER_H
#define CAPENGINE_TILEDTILELAYER_H

#include <filesystem>
#include <jsoncons/json.hpp>
#include <memory>
#include <optional>
#include <vector>

#include "camera2d.h"
#include "captypes.h"
#include "collision.h"
#include "tiledtileset.h"

namespace CapEngine
{
//! Tiled tile layer representation
/**
 * \brief Represents a tile layer from a Tiled map.
 * 
 * This class manages a single tile layer containing a 2D array of tiles.
 * It handles loading tile data from JSON, rendering tiles from tilesets,
 * and creating the final layer texture.
 */
class TiledTileLayer
{
   public:
    //! Global tile information
    /**
     * \brief Contains information about a tile's orientation and ID.
     */
    struct GlobalTileInfo {
        bool xFlip{false};               //!< Whether the tile is flipped horizontally
        bool yFlip{false};               //!< Whether the tile is flipped vertically
        bool diagFlip{false};            //!< Whether the tile is flipped diagonally
        unsigned int globalTileId{0};    //!< The global tile ID (across all tilesets)
    };

    explicit TiledTileLayer(
        const jsoncons::json& in_data,
        std::vector<std::unique_ptr<TiledTileset>>& in_tilesets,
        int in_tileWidth, int in_tileHeight, int in_mapWidth, int in_mapHeight,
        std::optional<std::filesystem::path> in_path = std::nullopt);

    static TiledTileLayer create(
        std::filesystem::path in_path,
        std::vector<std::unique_ptr<TiledTileset>>& in_tilesets,
        int in_tileWidth, int in_tileHeight, int in_mapWidth, int in_mapHeight);

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] int x() const;
    [[nodiscard]] int y() const;
    [[nodiscard]] bool visible() const;
    [[nodiscard]] std::vector<unsigned int> const& data() const;
    void render(uint32_t in_windowId) const;
    Texture* texture();

   private:
    std::optional<std::filesystem::path> m_path;          //!< Optional path to the layer file
    std::string m_name;                                   //!< Name of the layer
    int m_height{0};                                      //!< Height of the layer in tiles
    int m_width{0};                                       //!< Width of the layer in tiles
    int m_x{0};                                           //!< X offset of the layer
    int m_y{0};                                           //!< Y offset of the layer
    int m_mapHeight{0};                                   //!< Height of the parent map in tiles
    int m_mapWidth{0};                                    //!< Width of the parent map in tiles
    int m_tileWidth{0};                                   //!< Width of individual tiles in pixels
    int m_tileHeight{0};                                  //!< Height of individual tiles in pixels
    bool m_visible{true};                                 //!< Whether the layer is visible
    std::vector<unsigned int> m_data;                     //!< Raw tile data (global tile IDs)
    TexturePtr m_texture;                                 //!< Rendered texture of the layer
    std::vector<std::unique_ptr<TiledTileset>>& m_tilesets; //!< Reference to the map's tilesets
};

TiledTileLayer::GlobalTileInfo getGlobalTileInfo(unsigned int in_tileId);
}  // namespace CapEngine

#endif /* CAPENGINE_TILEDTILELAYER_H */
