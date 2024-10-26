#include "tiledobjectgroup.h"

#include <boost/throw_exception.hpp>
#include <optional>
#include <string>
#include <utility>

#include "CapEngineException.h"
#include "captypes.h"
#include "locator.h"
#include "jsoncons/basic_json.hpp"

namespace CapEngine
{

namespace {

void renderText(Texture* io_texture, const TiledObjectGroup::Text& in_text){

}

}  // namespace

TiledObjectGroup::Text::Text(const jsoncons::json& in_json)
{
    text = in_json["text"].as_string();
    fontfamily = in_json.get_value_or<std::string>("fontfamily", "");
    pixelsize = in_json.get_value_or<int>("pixelsize", 16);
    wrap = in_json.get_value_or<bool>("wrap", false);
    color = in_json.get_value_or<std::string>("color", "#000000");
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
      name(in_json.get_value_or<std::optional<std::string>>("name",
                                                            std::nullopt)),
      class_(in_json.get_value_or<std::optional<std::string>>("class",
                                                              std::nullopt)),
      visible(in_json["visible"].as_bool()),
      width(in_json["width"].as_double()),
      height(in_json["height"].as_double()),
      x(in_json["x"].as_double()),
      y(in_json["y"].as_double())
{
    if (in_json.contains("text")) {
        text = std::make_optional<Text>(in_json["text"]);
    }
}


TiledObjectGroup::TiledObjectGroup(const jsoncons::json& in_data,
                                   int in_width, int in_height,
                                   std::optional<std::filesystem::path> in_path)
    : m_path(in_path), m_mapWidth(in_width), m_mapHeight(in_height), m_texture(getNullTexturePtr())
{
    if (!in_data.contains("objects")) {
        BOOST_THROW_EXCEPTION(CapEngineException("Missing key \"objects\""));
    }

    for (const auto& objectJson : in_data["objects"].array_range()) {
        Object object{objectJson};
        std::string key =
            object.name != std::nullopt ? *(object.name) : object.id;
        m_objects.emplace(key, std::move(object));
    }
}

std::map<std::string, TiledObjectGroup::Object> const&
TiledObjectGroup::objects() const
{
    return m_objects;
}

std::map<std::string, TiledObjectGroup::Object>& TiledObjectGroup::objects()
{
    return m_objects;
}

std::optional<TiledObjectGroup::Object> TiledObjectGroup::objectByName(
    std::string_view in_name) const
{
    auto maybeObject =
        std::find_if(m_objects.begin(), m_objects.end(),
                     [in_name](const std::pair<std::string, Object>& in_pair) {
                         return in_pair.second.name == in_name;
                     });

    if (maybeObject != m_objects.end()) {
        return maybeObject->second;
    } else {
        return std::nullopt;
    }
}

void TiledObjectGroup::render(Texture* io_texture){
    // draw all our text objects to a texture if not already done
    if(m_texture == nullptr){
        for(auto && object : m_objects){
            if(object.second.text.has_value()){
                renderText(m_texture.get(), object.second.text.value());
            }
        }
    }

    // draw the combined text texture to the input texture
    SDL_Rect rect{0, 0, m_mapWidth, m_mapHeight};
    Locator::getVideoManager().drawTexture(io_texture, m_texture.get(), rect, rect);
}

}  // namespace CapEngine
