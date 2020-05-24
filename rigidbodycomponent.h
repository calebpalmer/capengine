#ifndef CAPENGINE_RIGIDBODYCOMPONENT_H
#define CAPENGINE_RIGIDBODYCOMPONENT_H

#include "components.h"

#include <jsoncons/json.hpp>

namespace CapEngine {

class ComponentFactory; // forward declaration

class RigidBodyComponent : public PhysicsComponent {
public:
	RigidBodyComponent(double in_mass);
	~RigidBodyComponent() override = default;

	std::unique_ptr<Component> clone() const override;

	static std::unique_ptr<RigidBodyComponent> makeComponent(const jsoncons::json &in_json);
	static void registerConstructor(ComponentFactory &in_factory);
	
	void update(GameObject &object, double timestep) override;

public:
	//! The component type.
	static constexpr inline char kType[] = "RigidBodyComponent";

private:
	double m_mass; //<! The mass of the object.
};

}

#endif // CAPENGINE_RIGIDBODYCOMPONENT_H
