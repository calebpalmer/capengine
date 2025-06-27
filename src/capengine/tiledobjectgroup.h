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
//! Tiled object group representation
/**
 * \brief Represents an object group from a Tiled map.
 * 
 * This class manages collections of objects (text, tiles, shapes) that are
 * placed on a Tiled map. Objects can have custom properties and are used
 * for things like spawn points, collision areas, and UI elements.
 */
class TiledObjectGroup {
   public:
    //! Text object properties
    /**
     * \brief Represents text formatting properties for text objects.
     */
    struct Text {
        std::string text;          //!< The text content to display
        std::string fontfamily;    //!< Font family name
        int pixelsize = 10;        //!< Font size in pixels
        std::string colour;        //!< Text color (hex format)
        bool wrap = false;         //!< Whether text should wrap
        bool bold = false;         //!< Whether text should be bold
        bool italic = false;       //!< Whether text should be italic
        bool underline = false;    //!< Whether text should be underlined
        bool strikeout = false;    //!< Whether text should be struck out
        bool kerning = false;      //!< Whether kerning should be applied
        std::string halign;        //!< Horizontal alignment (left, center, right)
        std::string valign;        //!< Vertical alignment (top, center, bottom)

        explicit Text(const jsoncons::json& in_json);
    };

    //! Tiled object representation
    /**
     * \brief Represents a single object within an object group.
     * 
     * Objects can be text, tiles, or geometric shapes with custom properties.
     * They are positioned within the map and can be used for various purposes
     * such as spawn points, triggers, or UI elements.
     */
    struct Object {
        std::string id;                               //!< Unique identifier for the object
        std::optional<std::string> name;              //!< Optional name for the object
        std::optional<std::string> class_;            //!< Optional class/type for the object
        bool visible;                                 //!< Whether the object is visible
        double width;                                 //!< Width of the object in pixels
        double height;                                //!< Height of the object in pixels
        double x;                                     //!< X position of the object
        double y;                                     //!< Y position of the object
        std::optional<Text> text;                     //!< Optional text properties if this is a text object
        std::optional<uint32_t> gid;                  //!< Optional global tile ID if this is a tile object
        std::vector<TiledCustomProperty> properties;  //!< Custom properties attached to this object

        explicit Object(const jsoncons::json& in_json);
    };

    explicit TiledObjectGroup(const jsoncons::json& in_data, int in_mapWidth, int in_mapHeight,
                              std::vector<std::unique_ptr<TiledTileset>>& in_tilesets,
                              std::optional<std::filesystem::path> in_path = std::nullopt);

    [[nodiscard]] std::optional<std::string> name() const;
    [[nodiscard]] bool visible() const;

    [[nodiscard]] std::map<std::string, Object> const& objects() const;
    [[nodiscard]] std::map<std::string, Object>& objects();

    std::optional<Object> objectByName(std::string_view in_name) const;
    void render(Texture* io_texture);

   private:
    std::optional<std::filesystem::path> m_path;          //!< Optional path to the object group file
    std::map<std::string, Object> m_objects;              //!< All objects in this group, keyed by ID
    int m_id = 0;                                         //!< Unique ID of the object group
    std::optional<std::string> m_name;                    //!< Optional name of the object group
    int m_mapWidth;                                       //!< Width of the parent map in pixels
    int m_mapHeight;                                      //!< Height of the parent map in pixels
    TexturePtr m_texture;                                 //!< Rendered texture containing all objects
    std::vector<std::unique_ptr<TiledTileset>>& m_tilesets; //!< Reference to the map's tilesets
    std::vector<TiledCustomProperty> m_properties;        //!< Custom properties of the object group
    bool m_visible = false;                               //!< Whether the object group is visible
};

}  // namespace CapEngine

#endif /* CAPENGINE_TILEDOBJECTGROUP_H */
