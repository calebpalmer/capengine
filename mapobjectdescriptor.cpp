#include "mapobjectdescriptor.h"

namespace CapEngine
{

//! Constructor
/**
* \param objectCkass the class of object (enemy)
* \param objectType the type of object (basic)
* \param startingPosition the loccation where the object is spawned on the map
*/
MapObjectDescriptor::MapObjectDescriptor(const std::string &objectClass, const std::string &objectType,
											const CapEngine::Vector& startingPosition)
				: m_objectClass(objectClass), m_objectType(objectType), m_startingPosition(startingPosition)
{ }



}
