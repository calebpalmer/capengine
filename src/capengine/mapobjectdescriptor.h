#ifndef MAPOBJECTDESCRIPTOR_H
#define MAPOBJECTDESCRIPTOR_H

#include "vector.h"

#include <string>

namespace CapEngine
{

class MapObjectDescriptor
{
public:
  MapObjectDescriptor(const std::string &objectClass,
                      const std::string &objectType,
                      const CapEngine::Vector &startingPosition);

private:
  std::string m_objectClass; //!< The class of object (ie: enemy)
  std::string m_objectType;  //!< The type of object of the given class
  CapEngine::Vector m_startingPosition; //!< The spawn point of the object
};

} // namespace CapEngine

#endif // MAPOBJECTDESCRIPTOR_H
