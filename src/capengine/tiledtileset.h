#ifndef CAPENGINE_TILEDTILESET_H
#define CAPENGINE_TILEDTILESET_H

#include "collision.h"
#include "captypes.h"

#include <filesystem>
#include <jsoncons/json.hpp>
#include <optional>
#include <string>

namespace CapEngine {

class TiledTileset final {
   public:
    explicit TiledTileset(std::string in_name, const jsoncons::json& in_data, int in_firstGid,
                          std::optional<std::filesystem::path> in_path = std::nullopt);

    static TiledTileset create(std::string in_name, std::filesystem::path in_path, int in_firstGid);

    [[nodiscard]] const std::string& name() const;
    [[nodiscard]] const std::string& image() const;
    [[nodiscard]] int firstGid() const;
    [[nodiscard]] int imageHeight() const;
    [[nodiscard]] int imageWidth() const;
    [[nodiscard]] int tileHeight() const;
    [[nodiscard]] int tileWidth() const;
    void loadTexture();
    std::optional<Texture*> texture();
    [[nodiscard]] Rectangle tileRect(int tileId) const;

    void drawTile(uint32_t in_gid, Texture* io_texture, int in_x, int in_y,
                  std::optional<int> in_destWidth = std::nullopt, std::optional<int> in_destHeight = std::nullopt);

   private:
    std::string m_name;
    std::optional<std::filesystem::path> m_path;
    std::string m_image;
    int m_firstGid{0};
    int m_imageHeight{0};
    int m_imageWidth{0};
    int m_tileHeight{0};
    int m_tileWidth{0};
    TexturePtr m_texture;
};
} // namespace CapEngine

#endif /* CAPENGINE_TILEDTILESET_H */
