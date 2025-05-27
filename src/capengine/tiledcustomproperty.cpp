#include "tiledcustomproperty.h"

#include <boost/throw_exception.hpp>
#include <charconv>
#include <sstream>

namespace CapEngine
{

template <>
int TiledCustomProperty::as<int>() const
{
    int result = 0;
    const char* begin = this->value.data();
    const char* end = begin + this->value.size();
    auto [ptr, ec] = std::from_chars(begin, end, result);

    if (ec == std::errc()) {
        return result;
    }

    std::stringstream err;
    err << "Error converting value to integer " << this->value;
    BOOST_THROW_EXCEPTION(std::runtime_error{err.str()});
}

}  // namespace CapEngine
