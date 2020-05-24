#ifndef NULL_AI_COMPONENT
#define NULL_AI_COMPONENT

#include "gameobject.h"

#include <memory>

namespace CapEngine {

//! An AI Component that does nothing.
class NullAIComponent :  public AIComponent {
public:
	void update(GameObject& /*object*/, double ms) override {}

	std::unique_ptr<Component> clone() const override {
		return std::make_unique<NullAIComponent>();
	}
};

}
#endif
