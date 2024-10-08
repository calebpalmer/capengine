#ifndef CAPENGINE_TILEDMAP_H
#define CAPENGINE_TILEDMAP_H

#include <filesystem>
#include <jsoncons/json.hpp>

#include "tiledobjectgroup.h"
#include "tiledtilelayer.h"
#include "tiledtileset.h"

namespace CapEngine
{
class TiledMap final
{
   public:
    explicit TiledMap(
        const jsoncons::json &in_json,
        std::optional<std::filesystem::path> in_path = std::nullopt);
    explicit TiledMap(const std::filesystem::path &in_mapPath);

    [[nodiscard]] int tileHeight() const;
    [[nodiscard]] int tileWidth() const;
    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] const std::vector<std::unique_ptr<TiledTileset>> &tilesets()
        const;
    [[nodiscard]] const std::vector<TiledTileLayer> &layers() const;
    [[nodiscard]] const std::vector<TiledObjectGroup> &objectGroups() const;

    [[nodiscard]] std::optional<TiledTileset const *> tileset(
        unsigned int index) const;

    void render();
    Texture *texture();

   private:
    void loadJson(const jsoncons::json &in_json);

    std::optional<std::filesystem::path> m_path;
    TexturePtr m_texture;
    int m_tileHeight{0};
    int m_tileWidth{0};
    int m_width{0};
    int m_height{0};
    std::vector<std::unique_ptr<TiledTileset>> m_tilesets;
    std::vector<TiledTileLayer> m_layers;
    std::vector<TiledObjectGroup> m_objectGroups;
};
}  // namespace CapEngine

#endif /* CAPENGINE_TILEDMAP_H */
