#include "componentutils.h"

namespace CapEngine { namespace ComponentUtils {

//! Gets the string representation of a ComponentType
/** 
 \param in_componentType
   The component type.
 \return 
   The string.
*/
std::string componentTypeToString(ComponentType in_componentType){
	switch(in_componentType){
	case ComponentType::Physics:
		return "Physics";
	case ComponentType::Graphics:
		return "Graphics";
	case ComponentType::Input:
		return "Input";
	case ComponentType::Custom:
		return "Custom";
	case ComponentType::AI:
		return "AI";
	default:
		return "Unknown";
	}
}

}}
