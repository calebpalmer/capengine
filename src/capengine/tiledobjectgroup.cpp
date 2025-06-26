#include "tiledobjectgroup.h"

#include <boost/range/adaptor/reversed.hpp>
#include <boost/throw_exception.hpp>
#include <optional>
#include <string>
#include <utility>

#include "CapEngineException.h"
#include "captypes.h"
#include "jsoncons/basic_json.hpp"
#include "locator.h"
#include "logging.h"
#include "tiledcustomproperty.h"

namespace CapEngine {

namespace {

constexpr char const* kObjectGroupVisiblePropertyName = "capengine-objectgroup-visible";

void renderText(Texture* io_texture, const TiledObjectGroup::Object& in_object)
{
    if (!in_object.text.has_value()) {
        return;
    }

    // create a surface with the text
    Colour colour{in_object.text->colour};

    // get the font settings
    auto fontFamily =
        std::find_if(in_object.properties.begin(), in_object.properties.end(),
                     [](const TiledCustomProperty& in_property) { return in_property.name == "capengine-font-ttf"; });
    if (fontFamily == in_object.properties.end()) {
        BOOST_THROW_EXCEPTION(CapEngineException("Missing tiled object property \"capengine-font-ttf\""));
    }

    auto fontSizeProperty =
        std::find_if(in_object.properties.begin(), in_object.properties.end(),
                     [](const TiledCustomProperty& in_property) { return in_property.name == "capengine-font-size"; });
    if (fontSizeProperty == in_object.properties.end()) {
        BOOST_THROW_EXCEPTION(CapEngineException("Missing tiled object property \"capengine-font-size\""));
    }

    auto surface = Locator::getFontManager().getTextSurface(fontFamily->value, in_object.text->text,
                                                            fontSizeProperty->as<int>(), colour);
    auto& videoManager = Locator::getVideoManager();
    auto texture = videoManager.createTextureFromSurfacePtr(surface.get(), false);
    auto srcWidth = static_cast<int>(videoManager.getTextureWidth(texture.get()));
    auto srcHeight = static_cast<int>(videoManager.getTextureHeight(texture.get()));
    Rect srcRect{0, 0, srcWidth, srcHeight};
    Rect dstRect{static_cast<int>(in_object.x), static_cast<int>(in_object.y), srcWidth, srcHeight};

    videoManager.drawTexture(io_texture, texture.get(), dstRect, srcRect);
}

void renderTile(Texture* io_texture, const TiledObjectGroup::Object& in_object,
                std::vector<std::unique_ptr<TiledTileset>>& in_tilesets)
{
    for (auto&& tileset : boost::adaptors::reverse(in_tilesets)) {
        if (tileset->firstGid() <= *in_object.gid) {
            tileset->drawTile(*in_object.gid, io_texture, static_cast<int>(in_object.x), static_cast<int>(in_object.y),
                              in_object.width, in_object.height);
        }
    }
}

}  // namespace

TiledObjectGroup::Text::Text(const jsoncons::json& in_json)
{
    text = in_json["text"].as_string();
    fontfamily = in_json.get_value_or<std::string>("fontfamily", "");
    pixelsize = in_json.get_value_or<int>("pixelsize", 16);
    wrap = in_json.get_value_or<bool>("wrap", false);
    colour = in_json.get_value_or<std::string>("color", "#000000");
    bold = in_json.get_value_or<bool>("bold", false);
    italic = in_json.get_value_or<bool>("italic", false);
    underline = in_json.get_value_or<bool>("underline", false);
    strikeout = in_json.get_value_or<bool>("strikeout", false);
    kerning = in_json.get_value_or<bool>("kerning", true);
    halign = in_json.get_value_or<std::string>("halign", "left");
    valign = in_json.get_value_or<std::string>("valign", "top");
}

TiledObjectGroup::Object::Object(const jsoncons::json& in_json)
    : id(in_json["id"].as_string()),
      name(in_json.get_value_or<std::optional<std::string>>("name", std::nullopt)),
      class_(in_json.get_value_or<std::optional<std::string>>("class", std::nullopt)),
      visible(in_json["visible"].as_bool()),
      width(in_json["width"].as_double()),
      height(in_json["height"].as_double()),
      x(in_json["x"].as_double()),
      y(in_json["y"].as_double())  // tiled uses the bottom left as the origin but we want the top left
{
    if (in_json.contains("text")) {
        text = std::make_optional<Text>(in_json["text"]);
    }

    if (in_json.contains("gid")) {
        gid = std::make_optional<uint32_t>(in_json["gid"].as<uint32_t>());

        // tile objects have their origin as the bottom left rather than top left like non-tile objects on an object
        // layer.
        y = y - height;
    }

    if (in_json.contains("properties")) {
        for (const auto& property : in_json["properties"].array_range()) {
            properties.push_back(TiledCustomProperty{property["name"].as_string(), property["type"].as_string(),
                                                     property["value"].as_string()});
        }
    }

    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Loaded object with id " << id;
}

TiledObjectGroup::TiledObjectGroup(const jsoncons::json& in_data, int in_mapWidth, int in_mapHeight,
                                   std::vector<std::unique_ptr<TiledTileset>>& in_tilesets,
                                   std::optional<std::filesystem::path> in_path)
    : m_path(std::move(in_path)),
      m_mapWidth(in_mapWidth),
      m_mapHeight(in_mapHeight),
      m_texture(getNullTexturePtr()),
      m_tilesets(in_tilesets)
{
    m_id = in_data["id"].as_integer<int>();
    m_name = in_data.get_value_or<std::string>("name", "");

    if (!in_data.contains("objects")) {
        BOOST_THROW_EXCEPTION(CapEngineException("Missing key \"objects\""));
    }

    for (const auto& objectJson : in_data["objects"].array_range()) {
        Object object{objectJson};
        // std::string key = object.name != std::nullopt ? *(object.name) : object.id;
        // m_objects.emplace(key, std::move(object));
        m_objects.emplace(object.id, std::move(object));
    }

    m_texture = Locator::getVideoManager().createTexturePtr(m_mapWidth, m_mapHeight, Colour{0, 0, 0, 0});

    for (auto&& object : m_objects) {
        if (object.second.text.has_value()) {
            renderText(m_texture.get(), object.second);
        }

        if (object.second.gid.has_value()) {
            renderTile(m_texture.get(), object.second, m_tilesets);
        }
    }

    if (in_data.contains("properties")) {
        for (const auto& property : in_data["properties"].array_range()) {
            m_properties.push_back(TiledCustomProperty{property["name"].as_string(), property["type"].as_string(),
                                                       property["value"].as_string()});
            if (property["name"].as_string() == kObjectGroupVisiblePropertyName) {
                // if the object has the property "capengine-objectgroup-visible" set to false, we set the visible
                // property to false
                m_visible = property["value"].as_string_view() == "true";
            }
        }
    }

    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Loaded object group with id " << m_id << " with name "
                                                              << (m_name != std::nullopt ? *m_name : "Unknown") << "\n";
}

std::map<std::string, TiledObjectGroup::Object> const& TiledObjectGroup::objects() const { return m_objects; }

std::map<std::string, TiledObjectGroup::Object>& TiledObjectGroup::objects() { return m_objects; }

std::optional<TiledObjectGroup::Object> TiledObjectGroup::objectByName(std::string_view in_name) const
{
    auto maybeObject = std::find_if(
        m_objects.begin(), m_objects.end(),
        [in_name](const std::pair<std::string, Object>& in_pair) { return in_pair.second.name == in_name; });

    if (maybeObject != m_objects.end()) {
        return maybeObject->second;
    }

    return std::nullopt;
}

void TiledObjectGroup::render(Texture* io_texture)
{
    SDL_Rect rect{0, 0, m_mapWidth, m_mapHeight};
    Locator::getVideoManager().drawTexture(io_texture, m_texture.get(), rect, rect);
}

std::optional<std::string> TiledObjectGroup::name() const { return m_name; }

bool TiledObjectGroup::visible() const { return m_visible; }

}  // namespace CapEngine
