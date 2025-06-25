#ifndef CAPENGINE_TILEDOBJECTGROUP_H
#define CAPENGINE_TILEDOBJECTGROUP_H

#include <filesystem>
#include <jsoncons/json.hpp>
#include <string>
#include <string_view>

#include "captypes.h"
#include "tiledcustomproperty.h"
#include "tiledtileset.h"

namespace CapEngine {
class TiledObjectGroup {
   public:
    struct Text {
        std::string text;
        std::string fontfamily;
        int pixelsize = 10;
        std::string colour;
        bool wrap = false;
        bool bold = false;
        bool italic = false;
        bool underline = false;
        bool strikeout = false;
        bool kerning = false;
        std::string halign;
        std::string valign;

        explicit Text(const jsoncons::json& in_json);
    };

    struct Object {
        std::string id;
        std::optional<std::string> name;
        std::optional<std::string> class_;
        bool visible;
        double width;
        double height;
        double x;
        double y;
        std::optional<Text> text;
        std::optional<uint32_t> gid;
        std::vector<TiledCustomProperty> properties;

        explicit Object(const jsoncons::json& in_json);
    };

    explicit TiledObjectGroup(const jsoncons::json& in_data, int in_mapWidth, int in_mapHeight,
                              std::vector<std::unique_ptr<TiledTileset>>& in_tilesets,
                              std::optional<std::filesystem::path> in_path = std::nullopt);

    [[nodiscard]] std::optional<std::string> name() const;

    [[nodiscard]] std::map<std::string, Object> const& objects() const;
    [[nodiscard]] std::map<std::string, Object>& objects();

    std::optional<Object> objectByName(std::string_view in_name) const;
    void render(Texture* io_texture);

   private:
    std::optional<std::filesystem::path> m_path;
    std::map<std::string, Object> m_objects;
    int m_id = 0;
    std::optional<std::string> m_name;
    int m_mapWidth;
    int m_mapHeight;
    TexturePtr m_texture;
    std::vector<std::unique_ptr<TiledTileset>>& m_tilesets;
};

}  // namespace CapEngine

#endif /* CAPENGINE_TILEDOBJECTGROUP_H */
