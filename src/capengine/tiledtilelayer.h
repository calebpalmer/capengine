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

namespace CapEngine {
class TiledTileLayer
{
   public:
    struct GlobalTileInfo {
        bool xFlip{false};
        bool yFlip{false};
        bool diagFlip{false};
        unsigned int globalTileId{0};
    };

    explicit TiledTileLayer(
        const jsoncons::json& in_data,
        std::weak_ptr<std::vector<TiledTileset>> in_tilesets, int in_tileWidth,
        int in_tileHeight, int in_mapWidth, int in_mapHeight,
        std::optional<std::filesystem::path> in_path = std::nullopt);
    static TiledTileLayer create(
        std::filesystem::path in_path,
        std::weak_ptr<std::vector<TiledTileset>> in_tilesets, int in_tileWidth,
        int in_tileHeight, int in_mapWidth, int in_mapHeight);

    int width() const;
    int height() const;
    int x() const;
    int y() const;
    bool visible() const;
    const std::vector<unsigned int> data() const;
    void render(uint32_t in_windowId) const;
    Texture* texture();

   private:
    std::optional<std::filesystem::path> m_path;
    int m_height{0};
    int m_width{0};
    int m_x{0};
    int m_y{0};
    int m_mapHeight{0};
    int m_mapWidth{0};
    int m_tileWidth{0};
    int m_tileHeight{0};
    bool m_visible{true};
    std::vector<unsigned int> m_data;
    TexturePtr m_texture;
    std::weak_ptr<std::vector<TiledTileset>> m_tilesets;
};

TiledTileLayer::GlobalTileInfo getGlobalTileInfo(unsigned int in_data);
}  // namespace CapEngine

#endif /* CAPENGINE_TILEDTILELAYER_H */
