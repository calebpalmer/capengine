#ifndef NULL_CUSTOM_COMPONENT
#define NULL_CUSTOM_COMPONENT

#include "gameobject.h"

#include <memory>

namespace CapEngine {

  class NullCustomComponent :  public CustomComponent {
  public:
    void update(GameObject* object) override {}
		
    std::unique_ptr<CustomComponent> clone() const override {
      std::unique_ptr<CustomComponent> newCustomComponent(new NullCustomComponent);
      return std::move(newCustomComponent);
    }
  };

}
#endif
