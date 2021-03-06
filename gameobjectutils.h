#ifndef CAPENGINE_GAMEOBJECTUTILS_H
#define CAPENGINE_GAMEOBJECTUTILS_H

#include "CapEngineException.h"
#include "gameobject.h"

#include <jsoncons/json.hpp>

namespace CapEngine
{

//! Exception class for object creation errors.
struct ObjectCreationError : public CapEngineException {
  ObjectCreationError(const std::string &in_details,
                      const jsoncons::json &in_json);
};

GameObject makeObject(const jsoncons::json &in_json);

} // namespace CapEngine

#endif // CAPENGINE_GAMEOBJECTUTILS_H
