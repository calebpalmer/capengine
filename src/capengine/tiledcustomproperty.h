#ifndef CAPENGINE_TILEDCUSTOMPROPERTY_H
#define CAPENGINE_TILEDCUSTOMPROPERTY_H

#include <stdexcept>
#include <string>

namespace CapEngine {

struct TiledCustomProperty {
    std::string name;
    std::string type;
    std::string value;

    template <typename T> T as() const;
};

template <typename T> T TiledCustomProperty::as() const {
    throw std::runtime_error("Template not implemented.");
}

template<> [[nodiscard]] int TiledCustomProperty::as<int>() const;

} // namespace CapEngine

#endif /* CAPENGINE_TILEDCUSTOMPROPERTY_H */
