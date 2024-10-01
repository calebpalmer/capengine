#ifndef CAPENGINE_TILEDOBJECTGROUP_H
#define CAPENGINE_TILEDOBJECTGROUP_H

#include <filesystem>
#include <jsoncons/json.hpp>
#include <string_view>

namespace CapEngine
{
class TiledObjectGroup
{
   public:
    struct Object {
        std::string id;
        std::optional<std::string> name;
        std::optional<std::string> class_;
        bool visible;
        double width;
        double height;
        double x;
        double y;

        explicit Object(const jsoncons::json& in_json);
    };

    explicit TiledObjectGroup(
        const jsoncons::json& in_data,
        std::optional<std::filesystem::path> in_path = std::nullopt);

    [[nodiscard]] std::map<std::string, Object> const& objects() const;
    [[nodiscard]] std::map<std::string, Object>& objects();

    std::optional<Object> objectByName(std::string_view in_name) const;

   private:
    std::optional<std::filesystem::path> m_path;
    std::map<std::string, Object> m_objects;
};
}  // namespace CapEngine

#endif /* CAPENGINE_TILEDOBJECTGROUP_H */
