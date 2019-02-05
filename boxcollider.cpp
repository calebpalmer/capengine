#include "boxcollider.h"

#include "scene2dschema.h"
#include "scene2dutils.h"
#include "componentutils.h"
#include "componentfactory.h"

namespace CapEngine {

//! Constructor
/** 
 \param in_rectangle
   The rectangle.
*/
BoxCollider::BoxCollider(Rectangle in_rectangle)
	: m_box(std::move(in_rectangle))
{
	
}

//! Makes a BoxCollider from json.
/** 
 \param in_json
   The jsn
 \return 
   The constructed BoxCollider
*/
std::unique_ptr<BoxCollider> BoxCollider::makeComponent(const jsoncons::json &in_json){
	try{
		Rectangle rect = JSONUtils::readRectangle(in_json[Schema::Components::kBox]);
		return std::make_unique<BoxCollider>(std::move(rect));
	}
	catch(jsoncons::json_exception &e){
		throw ComponentCreationException(ComponentUtils::componentTypeToString(ComponentType::Physics), kType, in_json, e.what());
	}
}

//! Registers this component with the Component Factory
/** 
 \param in_factor
   The component factory.
*/
void BoxCollider::registerConstructor(ComponentFactory &in_factory){
	in_factory.registerComponentType(ComponentUtils::componentTypeToString(ComponentType::Physics), kType, makeComponent);
}

} // namespace CapEngine
