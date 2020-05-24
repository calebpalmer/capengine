#include "rigidbodycomponent.h"

#include "componentfactory.h"
#include "scene2dschema.h"

namespace CapEngine {

//!  Constructor.
/** 
 \param in_mass
   The mass of the object.
*/
RigidBodyComponent::RigidBodyComponent(double in_mass)
	: m_mass(in_mass)
{
}

//! \copydoc Component::clone
std::unique_ptr<Component> RigidBodyComponent::clone() const{
	return std::make_unique<RigidBodyComponent>(*this);
}

//! \copydoc Component::update
void RigidBodyComponent::update(GameObject &object, double timestep){
	// apply forces here.
}

//! Creates the component from json.
/** 
 \param in_json
   The json
 \return 
   The component.
*/
std::unique_ptr<RigidBodyComponent> RigidBodyComponent::makeComponent(const jsoncons::json &in_json){
	try{
		assert(in_json[Schema::Components::kComponentSubType] == kType);
		double mass = in_json[Schema::Components::kMass].as<double>();

		return std::make_unique<RigidBodyComponent>(mass);
	}
	catch(const jsoncons::json_exception &e){
		throw ComponentCreationException(ComponentType::Physics, kType, in_json, boost::diagnostic_information(e));
	}
}

//! Registers the component with the ComponentFactory
/** 
 \param in_factory
   The factory.
*/
void RigidBodyComponent::registerConstructor(ComponentFactory &in_factory){
	in_factory.registerComponentType(ComponentUtils::componentTypeToString(ComponentType::Physics), kType, makeComponent);
}

} // namespace CapEngine
