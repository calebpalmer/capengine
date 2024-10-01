#include "tiledobjectgroup.h"

#include <boost/throw_exception.hpp>
#include <optional>
#include <string>
#include <utility>

#include "CapEngineException.h"

namespace CapEngine
{

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
}

TiledObjectGroup::TiledObjectGroup(const jsoncons::json& in_data,
                                   std::optional<std::filesystem::path> in_path)
    : m_path(in_path)
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

}  // namespace CapEngine
