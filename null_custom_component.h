#ifndef NULL_CUSTOM_COMPONENT
#define NULL_CUSTOM_COMPONENT

#include "gameobject.h"

#include <memory>

namespace CapEngine {

  class NullCustomComponent :  public CustomComponent {
  public:
    virtual void update(GameObject* object) {}
    virtual CustomComponent* clone() const {
      std::unique_ptr<CustomComponent> newCustomComponent(new NullCustomComponent);
      return newCustomComponent.release();
    }
  };

}
#endif
