#ifndef CAPENGINE_PROPERTYCOMMON_H
#define CAPENGINE_PROEPRTYCOMMON_H

namespace CapEngine
{

//! Enumeration to represent the type a property is holding.
enum class PropertyType {
  String,
  Int64,
  UInt64,
  Int32,
  UInt32,
  Float64,
  Float32,
  List
};

} // namespace CapEngine

#endif // CAPENGINE_PROPERTYCOMMON_H
