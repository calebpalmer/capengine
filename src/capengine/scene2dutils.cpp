#include "scene2dutils.h"

#include "CapEngineException.h"
#include "scene2dschema.h"

#include <jsoncons/json.hpp>

namespace CapEngine
{
namespace JSONUtils
{

//! Read a rectangle from json.
/**
 \param in_json
   The json to read the rectangle from.
 \return
   The rectangle
*/
Rectangle readRectangle(const jsoncons::json &in_json) {
  try {
    using namespace Schema::Scene2d;

    Rectangle rect;
    rect.x = in_json[kXCoord].as<int>();
    rect.y = in_json[kYCoord].as<int>();
    rect.width = in_json[kWidth].as<int>();
    rect.height = in_json[kHeight].as<int>();
    return rect;
  } catch (std::exception &e) {
    std::stringstream msg;
    msg << "Unable to parse rectangle from json." << std::endl
        << jsoncons::pretty_print(in_json) << std::endl
        << boost::diagnostic_information(e) << std::endl;
    BOOST_THROW_EXCEPTION(CapEngineException(msg.str()));
  }
}

//! Read a vector from json.
/**
 \param in_json
   The json to read the vector from.
 \return
   The vector.  All dimensions are optional, defaulted to 0.0.
*/
Vector readVector(const jsoncons::json &in_json) {
  using namespace Schema::Scene2d;
  Vector vector;

  if (in_json.contains(kXCoord)) {
    vector.setX(in_json[kXCoord].as<double>());
  }

  if (in_json.contains(kYCoord)) {
    vector.setY(in_json[kYCoord].as<double>());
  }

  if (in_json.contains(kZCoord)) {
    vector.setZ(in_json[kZCoord].as<double>());
  }

  return vector;
}

//! Reads a colour from json.
/**
 \param in_json
   The json to rea the colour from.
 \return
   The colour.
*/
Colour readColour(const jsoncons::json &in_json) {
  using namespace Schema::Colour;
  Colour colour;

  if (in_json.contains(kRed)) {
    colour.m_r = in_json[kRed].as<uint8_t>();
  }

  if (in_json.contains(kGreen)) {
    colour.m_g = in_json[kGreen].as<uint8_t>();
  }

  if (in_json.contains(kBlue)) {
    colour.m_b = in_json[kBlue].as<uint8_t>();
  }

  if (in_json.contains(kAlpha)) {
    colour.m_a = in_json[kAlpha].as<uint8_t>();
  }

  return colour;
}

MetadataType readMetadata(const jsoncons::json &in_json) {
  if (in_json.is<int>())
    return in_json.as<int>();
  else if (in_json.is<double>())
    return in_json.as<double>();
  else if (in_json.is_string())
    return in_json.as_string();
  else
    CAP_THROW(CapEngine::CapEngineException("Unsupported Metadata Type"));
}

} // namespace JSONUtils
} // namespace CapEngine
