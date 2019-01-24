#include "gameobjectutils.h"
#include "componentfactory.h"
#include "scene2dschema.h"
#include "scene2dutils.h"

namespace CapEngine {

//! Constructor.
/** 
 \param in_details
   The details of the error.
 \param in_json
   The json of the object.
*/
ObjectCreationError::ObjectCreationError(const std::string &in_details, const jsoncons::json &in_json)
	: CapEngineException([&]()
											 {
												 std::stringstream msg;
												 msg << "Error creation game object." << std::endl
														 << "details: " << in_details << std::endl
														 << "json: " << jsoncons::pretty_print(in_json);
												 return msg.str();
											 }())
{
}

//! Creates a new object from a json string.
/** 
 \param in_json
   The json to create the object from.
 \return 
   The object.
*/
GameObject makeObject(const jsoncons::json &in_json){
	using namespace Schema::Scene2d;
	
	GameObject object;
	
	// position
	if(in_json.contains(kPosition)){
		object.setPosition(JSONUtils::readVector(in_json[kPosition]));
	}

	// orientation
	if(in_json.contains(kOrientation)){
		object.setOrientation(JSONUtils::readVector(in_json[kOrientation]));
	}

	// get its components
	ComponentFactory &componentFactory = ComponentFactory::getInstance();
	for(auto && componentJson : in_json[kComponents].array_range()){
		std::unique_ptr<Component> pComponent = componentFactory.makeComponent(componentJson);
		object.setComponent(std::move(pComponent));
	}

	return object;
}

}
