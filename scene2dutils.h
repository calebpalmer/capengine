#ifndef CAPENGINE_SCENE2DUTILS_H
#define CAPENGINE_SCENE2DUTILS_H

#include "collision.h"

#include <jsoncons/json.hpp>

namespace CapEngine { namespace Utils {

Rectangle readRectangle(const jsoncons::json &in_json);
    
}}

#endif // CAPENGINE_SCENE2DUTILS_H
