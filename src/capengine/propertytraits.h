#ifndef CAPENGINE_PROPERTYTRAITS_H
#define CAPENGINE_PROPERTYTRAITS_H

#include "property.h"

#include <string>
#include <cstdint>

namespace CapEngine::TypeTraits
{

// TypeTrait class for Properties
template <typename T> struct PropertyTraits;

template <> struct PropertyTraits<std::string> {
  static const PropertyType propertyType = PropertyType::String;
};

template <> struct PropertyTraits<int64_t> {
  static const PropertyType propertyType = PropertyType::Int64;
};

template <> struct PropertyTraits<int32_t> {
  static const PropertyType propertyType = PropertyType::Int32;
};

template <> struct PropertyTraits<uint64_t> {
  static const PropertyType propertyType = PropertyType::UInt64;
};

template <> struct PropertyTraits<uint32_t> {
  static const PropertyType propertyType = PropertyType::UInt32;
};

template <> struct PropertyTraits<float> {
  static const PropertyType propertyType = PropertyType::Float32;
};

template <> struct PropertyTraits<double> {
  static const PropertyType propertyType = PropertyType::Float64;
};

} // namespace CapEngine::TypeTraits

#endif // CAPENGINE_PROPERTYTRAITS_H
