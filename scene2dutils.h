#ifndef CAPENGINE_SCENE2DUTILS_H
#define CAPENGINE_SCENE2DUTILS_H

#include "collision.h"
#include "vector.h"
#include "colour.h"

#include <jsoncons/json.hpp>

namespace CapEngine { namespace JSONUtils {

Rectangle readRectangle(const jsoncons::json &in_json);
Vector readVector(const jsoncons::json &in_json);
Colour readColour(const jsoncons::json &in_json);
    
}}

#endif // CAPENGINE_SCENE2DUTILS_H
