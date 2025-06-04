#include "tiledobjectgroup.h"

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
    auto srcWidth = videoManager.getTextureWidth(texture.get());
    auto srcHeight = videoManager.getTextureHeight(texture.get());
    Rect srcRect{0, 0, static_cast<int>(srcWidth), static_cast<int>(srcHeight)};  // TODO get width and heights
    Rect dstRect{static_cast<int>(in_object.x), static_cast<int>(in_object.y), static_cast<int>(in_object.width),
                 static_cast<int>(in_object.height)};

    videoManager.drawTexture(io_texture, texture.get(), dstRect, srcRect);
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
      y(in_json["y"].as_double())
{
    if (in_json.contains("text")) {
        text = std::make_optional<Text>(in_json["text"]);
    }

    if (in_json.contains("properties")) {
        for (const auto& property : in_json["properties"].array_range()) {
            properties.push_back(TiledCustomProperty{property["name"].as_string(), property["type"].as_string(),
                                                     property["value"].as_string()});
        }
    }

    BOOST_LOG_SEV(CapEngine::log, boost::log::trivial::debug) << "Loaded object with id " << id;
}

TiledObjectGroup::TiledObjectGroup(const jsoncons::json& in_data, int in_width, int in_height,
                                   std::optional<std::filesystem::path> in_path)
    : m_path(in_path), m_mapWidth(in_width), m_mapHeight(in_height), m_texture(getNullTexturePtr())
{
    m_id = in_data["id"].as_integer<int>();
    m_name = in_data.get_value_or<std::string>("name", "");

    if (!in_data.contains("objects")) {
        BOOST_THROW_EXCEPTION(CapEngineException("Missing key \"objects\""));
    }

    for (const auto& objectJson : in_data["objects"].array_range()) {
        Object object{objectJson};
        std::string key = object.name != std::nullopt ? *(object.name) : object.id;
        m_objects.emplace(key, std::move(object));
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
    // draw all our text objects to a texture if not already done
    if (m_texture == nullptr) {
        for (auto&& object : m_objects) {
            if (object.second.text.has_value()) {
                renderText(m_texture.get(), object.second);
            }
            else {
            }
        }
    }

    // draw the combined text texture to the input texture
    SDL_Rect rect{0, 0, m_mapWidth, m_mapHeight};
    Locator::getVideoManager().drawTexture(io_texture, m_texture.get(), rect, rect);
}

}  // namespace CapEngine
