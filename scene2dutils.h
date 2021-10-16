#ifndef CAPENGINE_SCENE2DUTILS_H
#define CAPENGINE_SCENE2DUTILS_H

#include "captypes.h"
#include "collision.h"
#include "colour.h"
#include "components.h"
#include "vector.h"

#include <jsoncons/json.hpp>

namespace CapEngine
{
namespace JSONUtils
{

Rectangle readRectangle(const jsoncons::json &in_json);
Vector readVector(const jsoncons::json &in_json);
Colour readColour(const jsoncons::json &in_json);
MetadataType readMetadata(const jsoncons::json &in_json);

} // namespace JSONUtils
} // namespace CapEngine

#endif // CAPENGINE_SCENE2DUTILS_H
